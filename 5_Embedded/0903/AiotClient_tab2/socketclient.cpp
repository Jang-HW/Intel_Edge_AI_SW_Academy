#include "socketclient.h"
#define Kor(str)  QString::fromLocal8Bit(str)

SocketClient::SocketClient(QWidget *parent, Qt::WindowFlags flags)
	: QWidget(parent, flags)
{
//    bool ok;
	pQTcpSocket = new QTcpSocket();

    // 소켓 연결시 발생하는 시그널
	connect(pQTcpSocket, SIGNAL(connected()), this, SLOT(slotConnectServer()));
    // 소켓 해제시
	connect(pQTcpSocket, SIGNAL(disconnected()), this, SLOT(slotClosedByServer()));
    // 수신시
	connect(pQTcpSocket, SIGNAL(readyRead()), this, SLOT(slotSocketReadData()));
    // 소켓 연결 오류시
	connect(pQTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotSocketError()));
//    slotConnectToServer(ok);
}


void SocketClient::slotConnectToServer(bool& ok){
    //서버 연결 요청
    QString strHostIp;
//	tcpSocket.connectToHost(QHostAddress::LocalHost, 5000);
    strHostIp = QInputDialog::getText(this,"HostIP","Input Server IP:",QLineEdit::Normal,SERVERIP,&ok);
    if(ok)
    {
        if(strHostIp.isEmpty())
            pQTcpSocket->connectToHost(SERVERIP, SERVERPOT);
        else
            pQTcpSocket->connectToHost(strHostIp, SERVERPOT);
    }
}
void SocketClient::slotConnectServer()
{
    QString Str = "["+LOGID+":"+LOGPW+"]";
    QByteArray byteStr = Str.toLocal8Bit();
    pQTcpSocket->write(byteStr);
}

void SocketClient::slotClosedByServer()
{
	pQTcpSocket->close();
}

// 데이터 수신시 동작
void SocketClient::slotSocketReadData(){
//    QList<QString> qList;
    QString strRecvData;
    // char이 cpp에서는 2바이트
        // 그래서 1바이트짜리인 배열이 따로 있음
    QByteArray qByteArray;

    // 데이터가 있는지 확인 / 너무 큰 데이터는 처리 안 함
	if (pQTcpSocket->bytesAvailable() > BLOCK_SIZE)
		return;
    // 받은 문자열을 가져와서
    qByteArray = pQTcpSocket->read(BLOCK_SIZE);
    // 문자열 포멧으로 형 변환
    strRecvData = QString::fromUtf8(qByteArray);
//    qDebug() << strRecvData;

    // 받은 메세지를 ui쪽으로 전달함
    emit sigSocketRecv(strRecvData);
//    QMessageBox::information(this,"Socket Recv",strRecvData);
}

void SocketClient::slotSocketError()
{
	QString Str = pQTcpSocket->errorString();
    qDebug() << Str;
}


void SocketClient::slotSocketSendData(QString strSendData)
{	
    if(!strSendData.isEmpty()) {
        strSendData = strSendData+"\n";
        QByteArray bCmd = strSendData.toLocal8Bit();
//        qDebug() << bCmd;
        pQTcpSocket->write(bCmd);
    }
}

QTcpSocket * SocketClient::getpQTcpSocket()
{
    return pQTcpSocket;
}

SocketClient::~SocketClient()
{
    pQTcpSocket->close();
}
