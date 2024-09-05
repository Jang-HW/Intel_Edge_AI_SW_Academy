#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    pTab1DevControl = new Tab1DevControl(ui->pTab1);
    ui->pTab1->setLayout(pTab1DevControl->layout());        // 레이아웃의 크기 자동으로 맞추기

    pTab2SocketClient = new Tab2SockeClient(ui->pTab2);
    ui->pTab2->setLayout(pTab2SocketClient->layout());

    pTab3ControlPannel = new Tab3ControlPannel(ui->pTab3);
    ui->pTab3->setLayout(pTab3ControlPannel->layout());

    pTab4ChartPlot = new Tab4ChartPlot(ui->pTab4);
    ui->pTab4->setLayout(pTab4ChartPlot->layout());

    pTab5Database = new Tab5Database(ui->pTab5);
    ui->pTab5->setLayout(pTab5Database->layout());

    pTab6WebCamera = new Tab6WebCamera(ui->pTab6);
    ui->pTab6->setLayout(pTab6WebCamera->layout());

    // 프로그램을 켜마자 보이는 탭 설정
    ui->pTabWidget->setCurrentIndex(5);

    // 각 탭을 추가하면서 기존에 작업한 내용과 겹치면
        // 시그널 슬롯을 통해서 관련 탭으로 보내주도록 작업을 할 것

    // tab2에서 수신 받은 led 값을 tab1에서 처리하기 위한 시그널
        // 외부에서 tab1의 private을 못 쓰니까, 다이얼 클래스를 받아와서 값을 변경함
    connect(pTab2SocketClient, SIGNAL(ledWriteSig(int)), pTab1DevControl->getpDial(), SLOT(setValue(int)));

    // tab2에서 수신 받은 센서 값을 tab4에 전달하기 위한 시그널
    connect(pTab2SocketClient, SIGNAL(tab4RecvDataSig(QString)), pTab4ChartPlot, SLOT(tab4RecvDataSlot(QString)));
    connect(pTab2SocketClient, SIGNAL(tab4RecvDataSig(QString)), pTab5Database, SLOT(tab5RecvDataSlot(QString)));

    // tab3에서 버튼을 누른 것에 따라서 tab2에서 메세지 전송
        // private인 socket을 getter로 얻어와서 그를 통해서 전송함
    connect(pTab3ControlPannel, SIGNAL(socketSendDataSig(QString)), pTab2SocketClient->getSocketClient(), SLOT(slotSocketSendData(QString)));
    connect(pTab2SocketClient, SIGNAL(tab3RecvDataSig(QString)), pTab3ControlPannel, SLOT(tab3RecvDataSlot(QString)));
}

MainWidget::~MainWidget()
{
    delete ui;
}

