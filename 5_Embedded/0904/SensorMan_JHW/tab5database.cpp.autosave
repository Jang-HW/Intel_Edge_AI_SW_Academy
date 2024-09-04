#include "tab5database.h"
#include "ui_tab5database.h"

Tab5Database::Tab5Database(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab5Database)
{
    ui->setupUi(this);
    // sql db 객체 생성
        // sqlite : 앱마다 table을 만들 수 있도록 하는 경량화된 db
    qSqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    // db 파일 이름
    qSqlDatabase.setDatabaseName("iot.db");
    if(qSqlDatabase.open())
        qDebug() << "succcess DB connection\n";
    else
        qDebug() << "fail DB connection\n";

    // table 생성 명령어
        // 초당 정보가 1번 이상 안들어와서
        // 시간을 primary key로 사용
    QString strQuery = "CREATE TABLE sensor_tb ("
                    "name VARCHAR(10),"
                    "date DATETIME primary key,"
                    "illu VARCHAR(10),"
                    "temp VARCHAR(10),"
                    "humi VARCHAR(10));";
    // 명령어 실행 - 오류 처리
    QSqlQuery sqlQuery;
    // table이 없으면 한번만 실행
    if(!sqlQuery.exec(strQuery))
        ;
    else
        qDebug() << "create table\n";

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

    connect(ui->pPBsearchDb,SIGNAL(clicked()), this, SLOT(searchDbSlot()));
    connect(ui->pPBdeleteDb,SIGNAL(clicked()), this, SLOT(deleteDbSlot()));
}

//
void Tab5Database::updateLastDateTime(int)
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    firstDate.setDate(date);
    firstDate.setTime(time);

    lastDate.setDate(date);
    QTime tempTime = time.addSecs(60*10);
    lastDate.setTime(tempTime);
    axisX->setRange(firstDate,lastDate);
}

// tab4랑 다르게 시간 축을 설정해줘야 됨
    // 설정한 시간대를 축의 정보로 갱신해 주는 함수
void Tab5Database::updateLastDateTimeSql(int)
{
    int row_count = 0;
    QDateTime from_dateTime = ui->pDateTimeEditFrom->dateTime();
    QString from_strDateTime = from_dateTime.toString("yyyy/MM/dd hh:mm:ss");
    QDateTime to_dateTime = ui->pDateTimeEditTo->dateTime();
    QString to_strDateTime = to_dateTime.toString("yyyy/MM/dd hh:mm:ss");

    // 쿼리문 저장
    QString strQuery = "select * from sensor_tb where date > '" + from_strDateTime + "' AND date < '" + to_strDateTime + "'";
    QSqlQuery sqlQuery;
    if(!sqlQuery.exec(strQuery)){

    } else {
//        qDebug() << "select query ok";
        // 제일 첫번째 기록으로 이동해서 날짜 부분을 가져옴
        sqlQuery.first();
        firstDate = QDateTime::fromString(sqlQuery.value("date").toString(), "yyyy/MM/dd hh:mm:ss");
        // 마지막 기록으로 이동해서 날짜 부분을 가져옴
        sqlQuery.last();
        lastDate = QDateTime::fromString(sqlQuery.value("date").toString(), "yyyy/MM/dd hh:mm:ss");
    }
    if(!firstDate.isValid())
        firstDate = QDateTime::fromString(from_strDateTime, "yyyy/MM/dd hh:mm:ss");
    if(!lastDate.isValid())
        lastDate = QDateTime::fromString(to_strDateTime, "yyyy/MM/dd hh:mm:ss");
//    qDebug() << firstDate;
//    qDebug() << lastDate;
    axisX->setRange(firstDate,lastDate);
}

// 수신된 메세지 처리
void Tab5Database::tab5RecvDataSlot(QString recvData)
{
    // @JHW_COM@SENSOR@조도@온도@습도

    qDebug() << "Tab5 " << recvData;
    QDateTime dateTime = QDateTime::currentDateTime();
    QStringList qlist = recvData.split("@");
    QString name = qlist[1];        //id
    QString illu = qlist[3];        //조도
    QString temp = qlist[4];        //온도
    QString humi = qlist[5];        //습도

    // 쿼리 구분 추가
        // 변수의 값을 쓰기 위해서 큰 따옴표를 잘 쓰기
    QString strQuery = "insert into sensor_tb(name, date, illu, temp, humi) values('" + name + "', '" + dateTime.toString("yyyy/MM/dd hh:mm:ss") + "' , '" + illu + "' , '" + temp + "' , '" + humi +"')";
    QSqlQuery sqlQuery;
    if(!sqlQuery.exec(strQuery)){
        ;
    } else {
        qDebug() << "insert query ok";
    }
}

//
void Tab5Database::searchDbSlot()
{
    int row_count = 0;
    QDateTime from_dateTime = ui->pDateTimeEditFrom->dateTime();
    QString from_strDateTime = from_dateTime.toString("yyyy/MM/dd hh:mm:ss");
    QDateTime to_dateTime = ui->pDateTimeEditTo->dateTime();
    QString to_strDateTime = to_dateTime.toString("yyyy/MM/dd hh:mm:ss");

    ui->pTBSensor->clearContents();
    illuLine->clear();
    updateLastDateTimeSql(0);

    QString strQuery = "select * from sensor_tb where date > '" + from_strDateTime + "' AND date < '" + to_strDateTime + "'";
    QSqlQuery sqlQuery;
    if(!sqlQuery.exec(strQuery)){
        ;
    } else {
//        qDebug() << "select query ok";

        while(sqlQuery.next()) {
            // 행은 1부터 시작
            row_count++;

            // 저장을 위한 객체를 생성
            ui->pTBSensor->setRowCount(row_count);
            QTableWidgetItem * pQTableWidgetItemId = new QTableWidgetItem();
            QTableWidgetItem * pQTableWidgetItemDate = new QTableWidgetItem();
            QTableWidgetItem * pQTableWidgetItemIllu = new QTableWidgetItem();
            QTableWidgetItem * pQTableWidgetItemTemp = new QTableWidgetItem();
            QTableWidgetItem * pQTableWidgetItemHumi = new QTableWidgetItem();

            // 객체에 값 지정
            pQTableWidgetItemId->setText(sqlQuery.value("name").toString());
            pQTableWidgetItemDate->setText(sqlQuery.value("date").toString());
            pQTableWidgetItemIllu->setText(sqlQuery.value("illu").toString());
            pQTableWidgetItemTemp->setText(sqlQuery.value("temp").toString());
            pQTableWidgetItemHumi->setText(sqlQuery.value("humi").toString());

            // 지정한 값을 저장
            ui->pTBSensor->setItem(row_count-1,0, pQTableWidgetItemId);
            ui->pTBSensor->setItem(row_count-1,1, pQTableWidgetItemDate);
            ui->pTBSensor->setItem(row_count-1,2, pQTableWidgetItemIllu);
            ui->pTBSensor->setItem(row_count-1,3, pQTableWidgetItemTemp);
            ui->pTBSensor->setItem(row_count-1,4, pQTableWidgetItemHumi);


            // 날짜 값을 정한 format으로 읽어옴
            QDateTime xValue = QDateTime::fromString(pQTableWidgetItemDate->text(), "yyyy/MM/dd hh:mm:ss");
            //
            illuLine->append(xValue.toMSecsSinceEpoch(),pQTableWidgetItemIllu->text().toInt());
            tempLine->append(xValue.toMSecsSinceEpoch(),pQTableWidgetItemTemp->text().toFloat());
            humiLine->append(xValue.toMSecsSinceEpoch(),pQTableWidgetItemHumi->text().toFloat());
//            qDebug() << "illu " << pQTableWidgetItemIllu->text().toInt();
        }
        // index를 제일 긴 문자열 기준으로 크기를 맞춤
        ui->pTBSensor->resizeColumnToContents(0);
        ui->pTBSensor->resizeColumnToContents(1);
        ui->pTBSensor->resizeColumnToContents(2);
        ui->pTBSensor->resizeColumnToContents(3);
        ui->pTBSensor->resizeColumnToContents(4);
    }
}

// 기간 내의 정보를 모두 지움
void Tab5Database::deleteDbSlot()
{
    QDateTime from_dateTime = ui->pDateTimeEditFrom->dateTime();
    QString from_strDateTime = from_dateTime.toString("yyyy/MM/dd hh:mm:ss");
    QDateTime to_dateTime = ui->pDateTimeEditTo->dateTime();
    QString to_strDateTime = to_dateTime.toString("yyyy/MM/dd hh:mm:ss");

    QString strQuery = "delete from sensor_tb where date > '" + from_strDateTime + "' AND date < '" + to_strDateTime + "'";
    QSqlQuery sqlQuery;
    if(!sqlQuery.exec(strQuery)){
        ;
    } else {
        qDebug() << "delete query ok";
    }
    // 내용을 지웠으니까 빈 공간을 삭제
    ui->pTBSensor->clearContents();
    // 지운 내용을 table에서 삭제
    illuLine->clear();
    tempLine->clear();
    humiLine->clear();
}

Tab5Database::~Tab5Database()
{
    delete ui;
}
