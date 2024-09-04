#include "customwidget.h"
#include "ui_customwidget.h"

CustomWidget::CustomWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CustomWidget)
{
    ui->setupUi(this);

    pTab2SocketClient = new Tab2SockeClient(ui->pTab1);
    ui->pTab1->setLayout(pTab2SocketClient->layout());

    pTab4ChartPlot = new Tab4ChartPlot(ui->pTab2);
    ui->pTab2->setLayout(pTab4ChartPlot->layout());

    pTab5Database = new Tab5Database(ui->pTab3);
    ui->pTab3->setLayout(pTab5Database->layout());


    // 프로그램을 켜마자 보이는 탭 설정
    ui->pTabWidget->setCurrentIndex(1);

    // tab2에서 수신 받은 센서 값을 tab4에 전달하기 위한 시그널
    connect(pTab2SocketClient, SIGNAL(tab4RecvDataSig(QString)), pTab4ChartPlot, SLOT(tab4RecvDataSlot(QString)));
    connect(pTab2SocketClient, SIGNAL(tab4RecvDataSig(QString)), pTab5Database, SLOT(tab5RecvDataSlot(QString)));
}

CustomWidget::~CustomWidget()
{
    delete ui;
}

