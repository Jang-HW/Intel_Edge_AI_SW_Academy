#include "tab2sockeclient.h"
#include "ui_tab2sockeclient.h"

Tab2SockeClient::Tab2SockeClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab2SockeClient)
{
    ui->setupUi(this);
    pSocketClient = new SocketClient(this);
    connect(pSocketClient,SIGNAL(sigSocketRecv(QString)),this,SLOT(socketRecvUpdateSlot(QString)));
    ui->pPBSendButton->setEnabled(false);
}

Tab2SockeClient::~Tab2SockeClient()
{
    delete ui;
}

void Tab2SockeClient::on_pPBServerConnect_clicked(bool checked)
{
    bool bOk;
    if(checked)
    {
        pSocketClient->slotConnectToServer(bOk);
        if(bOk)
        {
            ui->pPBServerConnect->setText("서버 해제");
            ui->pPBSendButton->setEnabled(true);
        }
    }
    else
    {
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
       if(bOk)
           emit ledWriteSig(ledNo);
    }
}

void Tab2SockeClient::on_pPBSendButton_clicked()
{
    QString strRecvId = ui->pLERecvId->text();
    QString strSendData = ui->pLESendData->text();
    if(!strSendData.isEmpty())
    {
        if(strRecvId.isEmpty())
           strSendData = "[ALLMSG]"+ strSendData;
        else
           strSendData = "["+strRecvId+"]"+strSendData;

        pSocketClient->slotSocketSendData(strSendData);
        ui->pLESendData->clear();
    }
}
