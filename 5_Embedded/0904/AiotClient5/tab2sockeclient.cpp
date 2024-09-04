#include "tab2sockeclient.h"
#include "ui_tab2sockeclient.h"

Tab2SockeClient::Tab2SockeClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab2SockeClient)
{
    ui->setupUi(this);
    pSocketClient = new SocketClient(this);
    pKeyboard = new Keyboard();

    // ui에 입력하기 위해서 수신한 데이터를 시그널으로 전달
    connect(pSocketClient,SIGNAL(sigSocketRecv(QString)),this,SLOT(socketRecvUpdateSlot(QString)));
    ui->pPBSendButton->setEnabled(false);

    // 문자열 입력을 위한 키보드 창이 나오도록 하는 시그널
    connect(ui->pLERecvId, SIGNAL(selectionChanged()), this, SLOT(keyboardSlot()));
    connect(ui->pLESendData, SIGNAL(selectionChanged()), this, SLOT(keyboardSlot()));
}

Tab2SockeClient::~Tab2SockeClient()
{
    delete ui;
}

// 버튼을 누르면 on/off를 왔다 갔다하며 연결-해제하는 함수
void Tab2SockeClient::on_pPBServerConnect_clicked(bool checked)
{
    bool bOk;
    if(checked)
    {
        // 서버 연결
        pSocketClient->slotConnectToServer(bOk);
        if(bOk)
        {
            // 버튼의 문자 변경
            ui->pPBServerConnect->setText("서버 해제");
            ui->pPBSendButton->setEnabled(true);
        }
    }
    else
    {
        // 서버 연결 해제
        pSocketClient->slotClosedByServer();
        ui->pPBServerConnect->setText("서버 연결");
        ui->pPBSendButton->setEnabled(false);
    }
}

// 수신된 데이터를 UI단에서 처리하는 함수
void Tab2SockeClient::socketRecvUpdateSlot(QString strRecvData)
{
    // 시간 출력
    QTime time = QTime::currentTime();
    QString strTime = time.toString();
    // 수신한 문자열의 개행문자를 제거함
    strRecvData.chop(1);    //"\n" 제거
    strTime = strTime + " " + strRecvData;

    // 문자열 파싱
    ui->pTERecvData->append(strTime);
    //[KSH_QT]LED@0xff ==> @KSH_QT@LED@0xff
    strRecvData.replace("[","@");
    strRecvData.replace("]","@");
    QStringList qList = strRecvData.split("@");
        // 0 : NULL
        // 1 : KSH_QT
        // 2 : LED
        // 3 : 0xff

    // 받은 명령어를 확인
    if(qList[2].indexOf("LED") == 0)
    {
       bool bOk;
       int ledNo =qList[3].toInt(&bOk,16);
       if(bOk){
           // 다른 탭에 있는 값을 수정해야해서 시그널
           emit ledWriteSig(ledNo);
       }
    }
    else if(qList[2].indexOf("SENSOR") == 0)
    {
        // 탭4와 5에 값을 전달하기 위한 시그널
        emit tab4RecvDataSig(strRecvData);
        emit tab5RecvDataSig(strRecvData);
    }
}

// 송신 버튼을 눌렀을 때 동작하는 함수
void Tab2SockeClient::on_pPBSendButton_clicked()
{
    // 수신자랑 메세지를 가져옴
    QString strRecvId = ui->pLERecvId->text();
    QString strSendData = ui->pLESendData->text();

    // 보낼 문자열이 있는지 확인
    if(!strSendData.isEmpty())
    {
        // 수신자가 있는지 확인
        if(strRecvId.isEmpty())
           strSendData = "[ALLMSG]"+ strSendData;
        else
           strSendData = "["+strRecvId+"]"+strSendData;

        // 메세지 전송
        pSocketClient->slotSocketSendData(strSendData);
        ui->pLESendData->clear();
    }
}

// 키보드 입력창을 나오도록 하는 slot 함수
void Tab2SockeClient::keyboardSlot() {
    // 어떤 객체에서 호출했는지 입력해서 돌려줘야 됨
        // sender는 가장 조상 클래스 형태의 포인터로 주기 때문에 형 변환 필요
    QLineEdit *pQLineEdit = (QLineEdit *)sender();

    // private인 값에 접근해서 수정하기 위해서 setter 함수
    pKeyboard->setLineEdit(pQLineEdit);
    // 화면에 출력
    pKeyboard->show();
}
