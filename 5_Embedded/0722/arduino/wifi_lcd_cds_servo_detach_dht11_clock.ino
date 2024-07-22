/*
  WiFiEsp test: ClientTest
  http://www.kccistc.net/
  작성일 : 2022.12.19
  작성자 : IoT 임베디드 KSH
*/
#define DEBUG
//#define DEBUG_WIFI

#include <WiFiEsp.h>
#include <SoftwareSerial.h>
// 서브모터가 timer1을 써서 여기서 2를 사용함 
#include <MsTimer2.h>
// I2C모듈
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
// 온습도 센서용
#include <DHT.h>

#define AP_SSID "iot1"
#define AP_PASS "iot10000"
#define SERVER_NAME "10.10.141.81"
#define SERVER_PORT 5000  
#define LOGID "JHW_ARD"
#define PASSWD "PASSWD"

#define CDS_PIN A0          // 
#define BUTTON_PIN 2        // 버튼 핀 / 외부 인터럽트
#define LED_LAMP_PIN 3      // LAMP 핀
#define DHTPIN 4            // 
#define SERVO_PIN 5         // 서브모터 핀
#define MOTOR_PIN 6
#define WIFITX 9  //7:TX -->ESP8266 RX
#define WIFIRX 8  //8:RX-->ESP8266 TX
#define LED_BUILTIN_PIN 13  // 

#define CMD_SIZE 50
#define ARR_CNT 5
#define DHTTYPE DHT11         // DHT22이 더 정밀도가 높은데, 비쌈
bool timerIsrFlag = false;
boolean lastButton = LOW;     // 버튼의 이전 상태 저장
boolean currentButton = LOW;  // 버튼의 현재 상태 저장
boolean ledOn = false;        // LED의 현재 상태 (on/off)
boolean cdsFlag = false;

char sendId[10] = "JHW_ARD";
char sendBuf[CMD_SIZE];
char lcdLine1[17] = "Smart IoT By KSH";
char lcdLine2[17] = "WiFi Connecting!";

int cds = 0;
unsigned int secCount;
// 전류가 가만히 있지 않고 흔들림 > 모터 돌아가는 부분도 흔들림
// 서브 모터를 동작한 뒤의 얼
unsigned int myservoTime = 0;

char getSensorId[10];
// 요청 시간을 저장하기 위한 변수
int sensorTime;
float temp = 0.0;
float humi = 0.0;
// 시계에 크리스탈 발진기가 들어가는데,
// 이거가 1일 정도 돌리면 오차가 꽤 생김
bool updateTimeFlag = false;
typedef struct {
  int year;
  int month;
  int day;
  int hour;
  int min;
  int sec;
} DATETIME;
DATETIME dateTime = {0, 0, 0, 12, 0, 0};
DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial wifiSerial(WIFIRX, WIFITX);
WiFiEspClient client;
// 주소를 바꾸면 여러개의 LCD도 사용 가능 
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;

void setup() {
  lcd.init();
  lcd.backlight();
  lcdDisplay(0, 0, lcdLine1);
  lcdDisplay(0, 1, lcdLine2);

  // 시계 각도 계산
  pinMode(CDS_PIN, INPUT);    // 조도 핀을 입력으로 설정 (생략 가능)
  pinMode(BUTTON_PIN, INPUT);    // 버튼 핀을 입력으로 설정 (생략 가능)

  pinMode(LED_LAMP_PIN, OUTPUT);    // LED 핀을 출력으로 설정
  pinMode(LED_BUILTIN_PIN, OUTPUT); //D13

#ifdef DEBUG
  Serial.begin(115200); //DEBUG
#endif
  wifi_Setup();

  // 1000미리세크마다 인터럽트
  MsTimer2::set(1000, timerIsr); // 1000ms period
  MsTimer2::start();

  // 
  myservo.attach(SERVO_PIN);
  myservo.write(0);
  myservoTime = secCount;
  dht.begin();
}

void loop() {
  if (client.available()) {
    socketEvent();
  }

  if (timerIsrFlag) //1초에 한번씩 실행
  {
    timerIsrFlag = false;
    if (!(secCount % 5)) //5초에 한번씩 실행
    {
      // analog to digital convertor 
      // 아두이노는 10 bit
      // cds도 사실상 저항이라 했는데, 설명 (필기)
      // 0 ~ 1023사이의 값을 0 ~ 100 사이의 값으로 매핑해서 출력
      cds = map(analogRead(CDS_PIN), 0, 1023, 0, 100);

      // 50 기준으로 값이 변경될 시, 출력
      // 이거 안 쓰면, 매 5초마다 전송 
      if ((cds >= 30) && cdsFlag)
      {
        cdsFlag = false;
        sprintf(sendBuf, "[%s]CDS@%d\n", sendId, cds);
        client.write(sendBuf, strlen(sendBuf));
        client.flush();
        //        digitalWrite(LED_BUILTIN_PIN, HIGH);     // LED 상태 변경
      } 
      else if ((cds < 30) && !cdsFlag)
      {
        cdsFlag = true;
        sprintf(sendBuf, "[%s]CDS@%d\n", sendId, cds);
        client.write(sendBuf, strlen(sendBuf));
        // 서버로 전송
        client.flush();
        //        digitalWrite(LED_BUILTIN_PIN, LOW);     // LED 상태 변경
      }
      humi = dht.readHumidity();
      temp = dht.readTemperature();

#ifdef DEBUG
/*
// 조도 습도 온도 출력
            Serial.print("Cds: ");
            Serial.print(cds);
            Serial.print(" Humidity: ");
            Serial.print(humi);
            Serial.print(" Temperature: ");
            Serial.println(temp);
*/   
#endif
      // LCD에는 16문자까지 쓸 수 있음
      sprintf(lcdLine2, "C:%2d,T:%2d,H:%2d", cds, (int)temp, (int)humi);
      lcdDisplay(0, 1, lcdLine2);

      if (!client.connected()) {
        lcdDisplay(0, 1, "Server Down");
        server_Connect();
      }
    }
    // 서브모터의 값이 노이즈로 인해 흔들리는거를 방지하기 위해서 하는 장치
    if (myservo.attached() && myservoTime + 2 == secCount)
    {
      // 여기서는 탐지만, 실제 동작은 다른 쪽에서 
      myservo.detach();
    }

    // 센서 값을 센서 값을 측정할 시간마다, 값을 측정해 전송
    if (sensorTime != 0 && !(secCount % sensorTime ))
    {
      // 정수로 보내기
      //sprintf(sendBuf, "[%s]SENSOR@%d@%d@%d\r\n", getSensorId, cds, (int)temp, (int)humi);
      
      // 실수로 보내기 => 이거는 메모리를 비교적 씀
      char tempStr[5];
      char humiStr[5];
      dtostrf(humi, 4, 1, humiStr);  //50.0  4: 전체자리수, 1: 소수 이하 자리수
      dtostrf(temp, 4, 1, tempStr);  //25.1
      sprintf(sendBuf,"[%s]SENSOR@%d@%s@%s\r\n",getSensorId,cds,tempStr,humiStr);
      
      client.write(sendBuf, strlen(sendBuf));
      client.flush();
    }
    sprintf(lcdLine1, "%02d.%02d  %02d:%02d:%02d", dateTime.month, dateTime.day, dateTime.hour, dateTime.min, dateTime.sec );
    lcdDisplay(0, 0, lcdLine1);
    if (updateTimeFlag)
    {
      client.print("[GETTIME]\n");
      updateTimeFlag = false;
    }
  }

  // 손으로 버튼을 누르면 진동으로 인해 on~off 사이를 왔다갔다 함 
  // 콘덴서를 활용한 지연회로를 사용하는게 정석인데,
  // 그러기 힘들어서, 지연시간을 주는 방식의 코딩임
  currentButton = debounce(lastButton);   // 디바운싱된 버튼 상태 읽기
  if (lastButton == LOW && currentButton == HIGH)  // 버튼을 누르면...
  {
    ledOn = !ledOn;       // LED 상태 값 반전
    digitalWrite(LED_BUILTIN_PIN, ledOn);     // LED 상태 변경
    //    sprintf(sendBuf,"[%s]BUTTON@%s\n",sendId,ledOn?"ON":"OFF");
    // 읽어온 값을 문자열에 저장
    sprintf(sendBuf, "[HM_CON]GAS%s\n", ledOn ? "ON" : "OFF");
    client.write(sendBuf, strlen(sendBuf));
    client.flush();
  }
  lastButton = currentButton;     // 이전 버튼 상태를 현재 버튼 상태로 설정

}

// 수신한 data의 소켓이벤트 처리
void socketEvent()
{
  int i = 0;
  char * pToken;
  char * pArray[ARR_CNT] = {0};
  char recvBuf[CMD_SIZE] = {0};
  int len;

  sendBuf[0] = '\0';
  len = client.readBytesUntil('\n', recvBuf, CMD_SIZE);
  client.flush();
#ifdef DEBUG
  Serial.print("recv : ");
  Serial.print(recvBuf);
#endif
  pToken = strtok(recvBuf, "[@]");
  while (pToken != NULL)
  {
    pArray[i] =  pToken;
    if (++i >= ARR_CNT)
      break;
    pToken = strtok(NULL, "[@]");
  }
  //[JHW_ARD]LED@ON : pArray[0] = "JHW_ARD", pArray[1] = "LED", pArray[2] = "ON"
  // LCD에 명령어를 출력하기 위한 변경점
  if ((strlen(pArray[1]) + strlen(pArray[2])) < 16)
  {
    sprintf(lcdLine2, "%s %s", pArray[1], pArray[2]);
    lcdDisplay(0, 1, lcdLine2);
  }
  if (!strncmp(pArray[1], " New", 4)) // New Connected
  {
#ifdef DEBUG
    Serial.write('\n');
#endif
    strcpy(lcdLine2, "Server Connected");
    lcdDisplay(0, 1, lcdLine2);
    // 서버에서 getTime을 요청하기 위한 플래그
    updateTimeFlag = true;
    return ;
  }
  else if (!strncmp(pArray[1], " Alr", 4)) //Already logged
  {
#ifdef DEBUG
    Serial.write('\n');
#endif
    client.stop();
    server_Connect();
    return ;
  }
  else if (!strcmp(pArray[1], "LED")) {
    if (!strcmp(pArray[2], "ON")) {
      digitalWrite(LED_BUILTIN_PIN, HIGH);
    }
    else if (!strcmp(pArray[2], "OFF")) {
      digitalWrite(LED_BUILTIN_PIN, LOW);
    }
    sprintf(sendBuf, "[%s]%s@%s\n", pArray[0], pArray[1], pArray[2]);
  } 
  else if (!strcmp(pArray[1], "LAMP")) {
    if (!strcmp(pArray[2], "ON")) {
      digitalWrite(LED_LAMP_PIN, HIGH);
    }
    else if (!strcmp(pArray[2], "OFF")) {
      digitalWrite(LED_LAMP_PIN, LOW);
    }
    sprintf(sendBuf, "[%s]%s@%s\n", pArray[0], pArray[1], pArray[2]);
  } 


  // MOTOR 관련 명령어 수신
  else if(!strcmp(pArray[1],"MOTOR")) {
    int speed = atoi(pArray[2]);
    speed = map(speed, 0, 100, 0, 255);
    analogWrite(MOTOR_PIN, speed);
    sprintf(sendBuf, "[%s]%s@%s\n", pArray[0], pArray[1], pArray[2]);
  }


  else if (!strcmp(pArray[1], "GETSTATE")) {
    strcpy(sendId, pArray[0]);
    if (!strcmp(pArray[2], "LED")) {
      sprintf(sendBuf, "[%s]LED@%s\n", pArray[0], digitalRead(LED_BUILTIN_PIN) ? "ON" : "OFF");
    }
  }
  else if (!strcmp(pArray[1], "SERVO"))
  {
    myservo.attach(SERVO_PIN);
    myservoTime = secCount;
    if (!strcmp(pArray[2], "ON"))
      myservo.write(180);
    else
      myservo.write(0);
    sprintf(sendBuf, "[%s]%s@%s\n", pArray[0], pArray[1], pArray[2]);

  }
  // 센서 값을 지정 받은 초마다 송신
  else if (!strncmp(pArray[1], "GETSENSOR", 9)) {
    if (pArray[2] != NULL) {
      sensorTime = atoi(pArray[2]);
      strcpy(getSensorId, pArray[0]);
      return;
    } else {
      sensorTime = 0;
      sprintf(sendBuf, "[%s]%s@%d@%d@%d\n", pArray[0], pArray[1], cds, (int)temp, (int)humi);
    }
  }
  // GETTIME 
  else if(!strcmp(pArray[0],"GETTIME")) {  
    dateTime.year = (pArray[1][0]-0x30) * 10 + pArray[1][1]-0x30 ;
    dateTime.month =  (pArray[1][3]-0x30) * 10 + pArray[1][4]-0x30 ;
    dateTime.day =  (pArray[1][6]-0x30) * 10 + pArray[1][7]-0x30 ;
    dateTime.hour = (pArray[1][9]-0x30) * 10 + pArray[1][10]-0x30 ;
    // 11번이 요일
    dateTime.min =  (pArray[1][12]-0x30) * 10 + pArray[1][13]-0x30 ;
    dateTime.sec =  (pArray[1][15]-0x30) * 10 + pArray[1][16]-0x30 ;
//#ifdef DEBUG
    sprintf(sendBuf,"\nTime %02d.%02d.%02d %02d:%02d:%02d\n\r",dateTime.year,dateTime.month,dateTime.day,dateTime.hour,dateTime.min,dateTime.sec );
    Serial.println(sendBuf);
//#endif
    return;
  } 
  else
    return;

  client.write(sendBuf, strlen(sendBuf));
  client.flush();

#ifdef DEBUG
  Serial.print(", send : ");
  Serial.print(sendBuf);
#endif
}
void timerIsr()
{
  timerIsrFlag = true;
  secCount++;
  clock_calc(&dateTime);
}

// 시간 24시간 단위로 getTime을 요청함
void clock_calc(DATETIME *dateTime)
{
  int ret = 0;
  dateTime->sec++;          // increment second

  if(dateTime->sec >= 60)                              // if second = 60, second = 0
  { 
      dateTime->sec = 0;
      dateTime->min++; 
             
      if(dateTime->min >= 60)                          // if minute = 60, minute = 0
      { 
          dateTime->min = 0;
          dateTime->hour++;                               // increment hour
          if(dateTime->hour == 24) 
          {
            dateTime->hour = 0;
            updateTimeFlag = true;
          }
       }
    }
}

void wifi_Setup() {
  wifiSerial.begin(38400);
  wifi_Init();
  server_Connect();
}
void wifi_Init()
{
  do {
    WiFi.init(&wifiSerial);
    if (WiFi.status() == WL_NO_SHIELD) {
#ifdef DEBUG_WIFI
      Serial.println("WiFi shield not present");
#endif
    }
    else
      break;
  } while (1);

#ifdef DEBUG_WIFI
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(AP_SSID);
#endif
  while (WiFi.begin(AP_SSID, AP_PASS) != WL_CONNECTED) {
#ifdef DEBUG_WIFI
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(AP_SSID);
#endif
  }
  sprintf(lcdLine1, "ID:%s", LOGID);
  lcdDisplay(0, 0, lcdLine1);
  sprintf(lcdLine2, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  lcdDisplay(0, 1, lcdLine2);

#ifdef DEBUG_WIFI
  Serial.println("You're connected to the network");
  printWifiStatus();
#endif
}
int server_Connect()
{
#ifdef DEBUG_WIFI
  Serial.println("Starting connection to server...");
#endif

  if (client.connect(SERVER_NAME, SERVER_PORT)) {
#ifdef DEBUG_WIFI
    Serial.println("Connect to server");
#endif
    client.print("["LOGID":"PASSWD"]");
  }
  else
  {
#ifdef DEBUG_WIFI
    Serial.println("server connection failure");
#endif
  }
}
void printWifiStatus()
{
  // print thㅋ`e SSID of the network you're attached to

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
void lcdDisplay(int x, int y, char * str)
{
  int len = 16 - strlen(str);
  lcd.setCursor(x, y);
  lcd.print(str);
  for (int i = len; i > 0; i--)
    lcd.write(' ');
}
boolean debounce(boolean last)
{
  boolean current = digitalRead(BUTTON_PIN);  // 버튼 상태 읽기
  if (last != current)      // 이전 상태와 현재 상태가 다르면...
  {
    // 버튼값이 흔들리는 시간 대기
    delay(5);         // 5ms 대기 : 안 좋은 코드 / 바꾸기 위한 방법은 이후 설명 
    //  Debounce라는 예제에서, millis()라는 함수를 사용함 => delay 대신 사용

    current = digitalRead(BUTTON_PIN);  // 버튼 상태 다시 읽기
  }
  return current;       // 버튼의 현재 상태 반환
}
