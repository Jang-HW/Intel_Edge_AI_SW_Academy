#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    pTab1DevControl = new Tab1DevControl(ui->pTab1);
    // 레이아웃의 크기 자동으로 맞추기
    ui->pTab1->setLayout(pTab1DevControl->layout());

    pTab2SocketClient = new Tab2SockeClient(ui->pTab2);
    ui->pTab2->setLayout(pTab2SocketClient->layout());
    ui->pTabWidget->setCurrentIndex(1);

    // led 값을 변경하는 명령어를 처리하기 위한 시그널
        // 외부에서 private을 못 쓰니까, 다이얼 클래스를 받아와서 값을 변경함
    connect(pTab2SocketClient, SIGNAL(ledWriteSig(int)), pTab1DevControl->getpDial(), SLOT(setValue(int)));

    // 각 탭을 추가하면서 기존에 작업한 내용과 겹치면
        // 시그널 슬롯을 통해서 관련 탭으로 보내주도록 작업을 할 것

}

MainWidget::~MainWidget()
{
    delete ui;
}

