/*
 WiFiEsp test: ClientTest
http://www.kccistc.net/
작성일 : 2019.12.17 
작성자 : IoT 임베디드 KSH
*/

/*
Output

최대 32256 바이트 
- 코드 사용 가능 용량

전역 변수는 동적 메모리 702바이트(34%)를 사용
- 70% 넘어가면 스택 공간 때문에 동작 안 할수도 

플래쉬메모리
- 전원끄면 날아가는 부분 
- 메모리가 용량이 너무 작아서 malloc 쓰면 안됨

EEPROM
- 아두이노는 4K?
- 펌웨어의 환경값을 저장하기 위한 메모리
- 전원이 차단되어도 메모리의 정보가 날아가지 않도록 하기 위해서 
*/




#define DEBUG
// WIFI 디버깅을 위한 전역변수
// 기능은 많은데 플레쉬 메모리가 부족해질 경우를 대비해서 막아둔 것 
// 13274(1.3K) > 14184 (1.5K)
//#define DEBUG_WIFI
#define AP_SSID "iot0"
#define AP_PASS "iot00000"
#define SERVER_NAME "10.10.141.81"
#define SERVER_PORT 5000  
#define LOGID "JHW_ARD"
#define PASSWD "PASSWD"

// 아두이노에서는 아날로그 write (PWM)
// 0, 1 유아트
// 2, 3 외부 인터럽트
#define WIFIRX 6  // 6 : RX-->ESP8266 TX
#define WIFITX 7  // 7 : TX -->ESP8266 RX
#define LED_TEST_PIN 12
#define LED_BUILTIN_PIN 13

// 너무 긴 배열을 쓰려고 하면, 버퍼 오버플로우가 남
// TCP의 경우, 50바이트 보낸다고 바로 받는다는 보장은 못함
#define CMD_SIZE 50
// 동시 연결 가능 장치 수 
#define ARR_CNT 5

// 시리얼 wifi를 아두이노는 1개만 사용 가능
// 0, 1을 쓸 수 있는데, serial.print는 전부 주석처리 필요
// stm은 uart 3~6개까지 사용 가능 
#include "WiFiEsp.h"
#include "SoftwareSerial.h"
// 아두이노의 타이머는 3개 (0, 1, 2)
// - 0: 못 씀. 이미 다른데에서 쓰고 있음
// - 1, 2 : 쓸 수 있음
//  - 시간당 인터럽트 핸들러를 1번으로 돌림
#include <TimerOne.h>

char sendBuf[CMD_SIZE];
// 1초가 지났는지 확인하기 위한 변수
bool timerIsrFlag = false;

// 타이머 인터럽트의 발생 횟수 (시계 등에 활용 가능)
unsigned int secCount;
// 센서 정보를 얼마 간격으로 보낼지 지정영 (여기서는 안 씀)
int sensorTime;

// C++ 식 class임
SoftwareSerial wifiSerial(WIFIRX, WIFITX); 
WiFiEspClient client;

/*
아두이노 IDE의 코드 구조

int main(){
  setup();
  while (1){
    loop();
  }
}
*/

void setup() {
  // put your setup code here, to run once:
    pinMode(LED_TEST_PIN, OUTPUT);    //D12
    pinMode(LED_BUILTIN_PIN, OUTPUT); //D13
    // HW 시리얼임 : 디버그용으로 주로 씀
    Serial.begin(115200); //DEBUG
    wifi_Setup();
    // 단위는 마이크로 세크인데, 10^6이니까 1초 단위로 인터럽트를 발생시키는 설정
    Timer1.initialize(1000000);
    // ISR 설정임 
    // - while문을 돌리면 안 됨 > 너무 오래 걸림
    // - delay는 쓰면 안됨 > 다른 루프의 실행도 막아버려서 
    // - 인터럽트에서는 짧은거만 처리 필요 (선점형이라 뒤에 거를 안봄)
    //   - arm의 경우에는 우선순위가 있어서 그나마 나음(?)
    // - 주기적으로 실행할 필요가 있는 것은 main에서 처리 필요
    Timer1.attachInterrupt(timerIsr); // timerIsr to run every 1 seconds
}

void loop() {
  // put your main code here, to run repeatedly:
  // read 함수는 blocking함수라서 읽어온 정보가 있는지 확인해서 
  // 최대 속도로 계속 처리하는 부분
  if(client.available()) {
    // 문자열 처리
    socketEvent();
  }

  // ISR에서 1초 지나면 true
  if (timerIsrFlag)
  {
    timerIsrFlag = false; 
    // 5의 배수일때 => 5초에 한 번 실행
    if(!(secCount%5))
    {
      // 연결이 끊어진 경우, 
      if (!client.connected()) { 
        // 서버에 재 연결 시도 
        server_Connect();
      }
    }
  }
}

// 데이터가 도착하면 처리 
void socketEvent()
{
  int i=0;
  char * pToken;
  // 수신가능한 명령어 수와 연관
  char * pArray[ARR_CNT]={0};
  // 수신한 데이터 저장
  char recvBuf[CMD_SIZE]={0}; 
  int len;

  // '\n'이 들어올때까지 바이트를 읽음 
  // 타자기에서 유래
    // 줄을 바꾸고(라인 피드, '\n'), 제일 앞쪽으로 돌아가는 것(캐리지 리턴,  '\r)
    // 통신에서는 일반적으로 '\n'만 보내고, 나머지 '\r'도 보내고 싶으면 2바이트를 써야함
  // 통신 프로토콜은 직접 만들어서 쓸 수도 있는데, 일반적으로 가변 문자열의 경우
    // 보통 STX(1), 명령어(2), 문자열길이(2), 문자열(...), 체크섬(1), ETX(1)
      // 체크섬은 각 바이트를 XOR해서 연산 
  // 우리의 경우, 명령어에 인수 3개까지 전송 가능(CMD@?@?@?)
  len =client.readBytesUntil('\n',recvBuf,CMD_SIZE); 
//  recvBuf[len] = '\0';
  client.flush();
#ifdef DEBUG
  Serial.print("recv : ");
  Serial.print(recvBuf);
#endif
  // 문자열을 세 문자([@])를 기준으로 분리 
    //***** 이 함수는 일치한 글자를 NULL으로 바꿔버려서 찍어보면 문자열이 다름 *****//
  pToken = strtok(recvBuf,"[@]");
  // \n이 안 들어옴
  while(pToken != NULL)
  {
    pArray[i] =  pToken;
    if(++i >= ARR_CNT)
      break;
    pToken = strtok(NULL,"[@]");
  }

  // New Connected 수신
  if(!strncmp(pArray[1]," New",4))  
  {
    Serial.write('\n');
    return ;
  }
  // Already logged 수신
    // 아두이노 계정과 동일한 계정이 로그인 되어 있는 경우
  else if(!strncmp(pArray[1]," Alr",4)) 
  {
    Serial.write('\n');
    client.stop();
    server_Connect();
    return ;
  }   
  // LED 관련 명령어 수신
  else if(!strcmp(pArray[1],"LED")) {
    if(!strcmp(pArray[2],"ON")) {
      digitalWrite(LED_BUILTIN_PIN,HIGH);
    }
    else if(!strcmp(pArray[2],"OFF")) {
      digitalWrite(LED_BUILTIN_PIN,LOW);
    }
    // 명령어 응답을 주기 위한 명령어 작성 / 보낼클라이언트, 받은 명령어, 받은 상태
    sprintf(sendBuf,"[%s]%s@%s\n",pArray[0],pArray[1],pArray[2]);
  } 
  // LAMP 관련 명령어 수신
  else if(!strcmp(pArray[1],"LAMP")) {
    if(!strcmp(pArray[2],"ON")) {
      digitalWrite(LED_TEST_PIN,HIGH);
    }
    else if(!strcmp(pArray[2],"OFF"))
    {
      digitalWrite(LED_TEST_PIN,LOW);
    }
    sprintf(sendBuf,"[%s]%s@%s\n",pArray[0],pArray[1],pArray[2]);
  }
  // 상태를 알려달라는 요청 "GETSTATE"
  else if(!strcmp(pArray[1],"GETSTATE")) {
    if(!strcmp(pArray[2],"DEV")) {
      
      sprintf(sendBuf,"[%s]DEV@%s@%s\n",pArray[0],digitalRead(LED_BUILTIN_PIN)?"ON":"OFF",digitalRead(LED_TEST_PIN)?"ON":"OFF");
    }
  }
  client.write(sendBuf,strlen(sendBuf));
  client.flush();

#ifdef DEBUG
  Serial.print(", send : ");
  Serial.print(sendBuf);
#endif
}

void timerIsr()
{
//  digitalWrite(LED_BUILTIN_PIN,!digitalRead(LED_BUILTIN_PIN));
  timerIsrFlag = true;
  secCount++;
}

void wifi_Setup() {
  // 라이브러리의 메소드임
  wifiSerial.begin(38400);
  wifi_Init();
  server_Connect();
}

void wifi_Init()
{
  do {
    // 초기화
    WiFi.init(&wifiSerial);
    // 상태를 읽어와서 이 값이면, 통신이 안되는 것
    if (WiFi.status() == WL_NO_SHIELD) {
#ifdef DEBUG_WIFI    
      Serial.println("WiFi shield not present");
#endif 
    }
    // 다른 응답이면 연결된 것
    else
      break;   
  }while(1);

#ifdef DEBUG_WIFI    
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(AP_SSID);
#endif     
    // wifi 접속 시도
    while(WiFi.begin(AP_SSID, AP_PASS) != WL_CONNECTED) {   
#ifdef DEBUG_WIFI  
      Serial.print("Attempting to connect to WPA SSID: ");
      Serial.println(AP_SSID);   
#endif   
    }
#ifdef DEBUG_WIFI      
  Serial.println("You're connected to the network");    
  printWifiStatus();
#endif 
}

// 서버 연결을 시도하는 함수
int server_Connect()
{
#ifdef DEBUG_WIFI     
  Serial.println("Starting connection to server...");
#endif  
  // 라파에 연결 시도
  if (client.connect(SERVER_NAME, SERVER_PORT)) {
#ifdef DEBUG_WIFI     
    Serial.println("Connected to server");
#endif  

    // 서버에서 인증 시도
    client.print("["LOGID":"PASSWD"]"); 
  }
  else
  {
#ifdef DEBUG_WIFI      
     Serial.println("server connection failure");
#endif    
  } 
}

// WIFI 상태 출력 함수 
void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}