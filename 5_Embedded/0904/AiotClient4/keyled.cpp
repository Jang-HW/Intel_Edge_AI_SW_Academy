#include "keyled.h"


KeyLed::KeyLed(QWidget *parent) :
    QWidget(parent)
{
    // cpp에서 파일 제어
    pFile = new QFile("/dev/ledkey");
    // 표준 파일 입출력 fopen -> 버퍼 유
    // 저수준 파일 입출력 open -> 버퍼 무
        // 저수준 파일 입출력을 하기 위해서 Unbuffered 옵션을 줌
    if(!pFile->open(QFile::ReadWrite | QFile::Unbuffered))
    {
        // 간단한 오류 출력용 창
        QMessageBox::information(this,"open","open fail : /dev/ledkey");
    }
    // fd number를 받아옴
    keyledFd = pFile->handle();
    pKeyLedNotifier = new QSocketNotifier(keyledFd,QSocketNotifier::Read,this);
    // 파일을 읽는다면, readKeyData로 처리
    connect(pKeyLedNotifier,SIGNAL(activated(int)),this,SLOT(readKeyData(int)));

    // led 값을 0으로 초기화
    writeLedData(0);
}
void KeyLed::readKeyData(int)
{
    char no = 0;
    int ret = pFile->read(&no,sizeof(no));
    // 누른 버튼 값 출력
    qDebug() << int(no);
    if(ret > 0){
        // 다른 시그널을 발생시켜 ui를 처리할 수 있는 곳으로 넘김
        emit updateKeydataSig(int(no));
    }
}

void KeyLed::writeLedData(int no)
{
    char led = (char)no;
    pFile->write(&led, sizeof(led));
}

KeyLed::~KeyLed()
{

}
