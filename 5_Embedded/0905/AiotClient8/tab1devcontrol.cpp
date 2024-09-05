#include "tab1devcontrol.h"
#include "ui_tab1devcontrol.h"

Tab1DevControl::Tab1DevControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab1DevControl)
{
    ui->setupUi(this);    

    // led 연결
    pKeyLed = new KeyLed(this);

    // 타이머의 값을 받아오기 위한 개체
    pQTimer = new QTimer(this);

    // 체크 박스를 묶는 그룹
    pQButtonGroup = new QButtonGroup(this);
    int itemCnt = ui->gridLayout->rowCount() * ui->gridLayout->columnCount();
    for (int i = 0; i < ui->gridLayout->rowCount(); i++) {
        for (int k = 0; k < ui->gridLayout->columnCount(); k++) {
            pQCheckBoxArray[--itemCnt] = dynamic_cast<QCheckBox*>(ui->gridLayout->itemAtPosition(i, k)->widget());
        }
    }
    itemCnt = ui->gridLayout->rowCount() * ui->gridLayout->columnCount();
    for (int i = 0; i < itemCnt; i++) {
        pQButtonGroup->addButton(pQCheckBoxArray[i], i+1);
    }
    // 그룹 내에서 여러 개의 체크박스를 누를 수 있도록 함
    pQButtonGroup->setExclusive(false);


    connect(ui->pDialLed, SIGNAL(valueChanged(int)), pKeyLed, SLOT(writeLedData(int)));
    connect(ui->pDialLed, SIGNAL(valueChanged(int)), ui->pLcdNumberLed, SLOT(display(int)));
    connect(ui->pPBtimerStart, SIGNAL(clicked(bool)), this, SLOT(slotTimerStartButton(bool)));
    // time-out signal 연결
    connect(pQTimer, SIGNAL(timeout()), this, SLOT(slotDialSetValue()));
    // 물리 버튼을 눌렀을 때의 신호
    connect(pKeyLed, SIGNAL(updateKeydataSig(int)), this, SLOT(slotKeyCheckBoxUpdate(int)));    
    // 체크 박스 이벤트 설정
    connect(pQButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotKeyCheckBoxUpdateMouse(int)));
    // led 값이 변경되었다는 신호가 오면 pKeyLed에서 처리
    connect(this, SIGNAL(writeLedDataSig(int)), pKeyLed, SLOT(writeLedData(int)));
}

Tab1DevControl::~Tab1DevControl()
{
    delete ui;
}

// quit 버튼 이벤트
void Tab1DevControl::on_pPBquit_clicked()
{
    qApp->quit();
}

// 타이머 시작/정지 버튼 이벤트
void Tab1DevControl::slotTimerStartButton(bool bCheck){
    if(bCheck){
        QString strValue = ui->pCBtimerValue->currentText();

        // time-out interrupt를 발생시키는 타이머 설정
        pQTimer->start(strValue.toInt());
        ui->pPBtimerStart->setText("TimerStop");
    }
    else {
        // time-out interrupt를 발생시키는 타이머 설정
        pQTimer->stop();
        ui->pPBtimerStart->setText("TimerStart");
    }
}

// time-out시 처리할 함수 정의
void Tab1DevControl::slotDialSetValue(){
    int dialValue = ui->pDialLed->value();

    if (dialValue >= ui->pDialLed->maximum())   dialValue = 0;
    else                                        dialValue++;

    ui->pDialLed->setValue(dialValue);
}

// 타이머의 시간 변경시 바로 반영되도록 타이머 값 변경
void Tab1DevControl::on_pCBtimerValue_currentIndexChanged(const QString &arg1)
{
    if(pQTimer->isActive()) {
        pQTimer->stop();
        pQTimer->start(arg1.toInt());
    }
}

// 물리 버튼 누르는 것에 따라 값을 갱신
void Tab1DevControl::slotKeyCheckBoxUpdate(int keyNo){
    lcdData = lcdData ^ (0x01 << (keyNo -1));
    ui->pLcdNumberKey->display(lcdData);

    for(int i=0; i<8; i++){
        if(keyNo == i+1){
            if(pQCheckBoxArray[i]->isChecked())
                pQCheckBoxArray[i]->setChecked(false);
            else
                pQCheckBoxArray[i]->setChecked(true);
        }
    }

    // led에 눌린 값을 쓰기
//    pKeyLed->writeLedData(lcdData);
    emit writeLedDataSig(lcdData);
    ui->pDialLed->setValue(lcdData);
}

// 체크 버튼을 클릭 하는 것에 따라 값을 갱신
void Tab1DevControl::slotKeyCheckBoxUpdateMouse(int keyNo){
    lcdData = lcdData ^ (0x01 << (keyNo -1));
    ui->pLcdNumberKey->display(lcdData);
}

QDial *Tab1DevControl::getpDial() {
    return ui->pDialLed;
}
