#include "tab4chartplot.h"
#include "ui_tab4chartplot.h"

Tab4ChartPlot::Tab4ChartPlot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab4ChartPlot)
{
    ui->setupUi(this);
    illuLine = new QLineSeries(this);
    illuLine->setName("조도");
    tempLine = new QLineSeries(this);
    tempLine->setName("온도");
    humiLine = new QLineSeries(this);
    humiLine->setName("습도");

    // 차트 색을 바꾸기 위한 펜이란 객체 설정
    QPen pen;
    pen.setWidth(2);
    pen.setBrush(Qt::red);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);
    // 데이터에 속성 적용
    illuLine->setPen(pen);

    pen.setBrush(Qt::green);
    tempLine->setPen(pen);

    pen.setBrush(Qt::blue);
    humiLine->setPen(pen);

    // 차트 생성 후, 데이터 적용
    chart = new QChart();
//   chart->legend()->hide();
    chart->addSeries(illuLine);
    chart->addSeries(tempLine);
    chart->addSeries(humiLine);

    // 차트 이름 설정
//    chart->setTitle("illumination chart");
    // 기본축으로 설정
    chart->createDefaultAxes();
    // 세로 축 값 범위 설정
    chart->axes(Qt::Vertical).first()->setRange(0, 100);
//    chart->axes(Qt::Horizontal).first()->setRange(0, 24);

    // 차트 뷰
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->pChartViewLayout->layout()->addWidget(chartView);

    // x축을 시간 축으로 변경을 위한 설정
    axisX = new QDateTimeAxis;
//    axisX->setFormat("MM-dd hh:mm");
    axisX->setFormat("hh:mm");
    // x축에서 사용할 시간 정보 업데이트 (firstDate, lastDate)
    updateLastDateTime(0);
    // x축 적용
    chartView->chart()->setAxisX(axisX,illuLine);
    chartView->chart()->setAxisX(axisX,tempLine);
    chartView->chart()->setAxisX(axisX,humiLine);

    // clear 버튼을 누르면, 버튼 누른 시간부터 축 재 설정
    connect(ui->pPBClearButton,SIGNAL(clicked()),this,SLOT(chartPlotClear()));
}

// x축 시간 설정 함수
void Tab4ChartPlot::updateLastDateTime(int)
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    // 호출 당시의 시간 설정
    firstDate.setDate(date);
    firstDate.setTime(time);

    // 호출 시간에 10분 늘려서 저장
    lastDate.setDate(date);
    QTime tempTime = time.addSecs(60*10);
    lastDate.setTime(tempTime);
    axisX->setRange(firstDate,lastDate);
}

void Tab4ChartPlot::chartPlotClear()
{
    illuLine->clear();
    tempLine->clear();
    humiLine->clear();
    updateLastDateTime(0);
}

// 수신한 데이터를 추가하는 함수
void Tab4ChartPlot::tab4RecvDataSlot(QString recvData)
{

    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QDateTime xValue;

    // 현재 시간과 날짜를 저장
    xValue.setDate(date);
    xValue.setTime(time);

    // 데이터 파싱
        // @HM_CON@SENSOR@12.3@4.56@78.9  //조도,온도,습도
    QStringList qlist = recvData.split("@");
        // qlist = {"", "HM_CON", "SENSOR", "12.3", "4.56", "78.9"}
    QString illu = qlist[3];        //조도
    QString temp = qlist[4];        //온도
    QString humi = qlist[5];        //습도
//    qDebug() << xValue.toMSecsSinceEpoch();

    // 축에는 분 단위로 보이는데, 처리는 초단위로 저장함
    illuLine->append(xValue.toMSecsSinceEpoch(),illu.toInt());
    tempLine->append(xValue.toMSecsSinceEpoch(),temp.toFloat());
    humiLine->append(xValue.toMSecsSinceEpoch(),humi.toFloat());
}

Tab4ChartPlot::~Tab4ChartPlot()
{
    delete ui;
}
