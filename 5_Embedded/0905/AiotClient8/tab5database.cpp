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
    qSqlDatabase.setDatabaseName("aiot.db");
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
                    "illu VARCHAR(10))";
    // 명령어 실행 - 오류 처리
    QSqlQuery sqlQuery;
    // table이 없으면 한번만 실행
    if(!sqlQuery.exec(strQuery))
        ;
    else
        qDebug() << "create table\n";

    illuLine = new QLineSeries(this);
    illuLine->setName("조도");

    QPen pen;
    pen.setWidth(2);
    pen.setBrush(Qt::red);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);
    illuLine->setPen(pen);
    chart = new QChart();
//   chart->legend()->hide();
    chart->addSeries(illuLine);

//    chart->setTitle("illumination chart");
    chart->createDefaultAxes();
    chart->axes(Qt::Vertical).first()->setRange(0, 100);
//    chart->axes(Qt::Horizontal).first()->setRange(0, 24);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->pChartViewLayout->layout()->addWidget(chartView);

    axisX = new QDateTimeAxis;
//    axisX->setFormat("MM-dd hh:mm");
    axisX->setFormat("hh:mm");

    updateLastDateTimeSql(0);

    chartView->chart()->setAxisX(axisX,illuLine);

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

    // 쿼리 구분 추가
        // 변수의 값을 쓰기 위해서 큰 따옴표를 잘 쓰기
    QString strQuery = "insert into sensor_tb(name, date,illu) values('" + name + "', '" + dateTime.toString("yyyy/MM/dd hh:mm:ss") + "' , '" + illu +"')";

    //
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

            // 객체에 값 지정
            pQTableWidgetItemId->setText(sqlQuery.value("name").toString());
            pQTableWidgetItemDate->setText(sqlQuery.value("date").toString());
            pQTableWidgetItemIllu->setText(sqlQuery.value("illu").toString());

            // 지정한 값을 저장
            ui->pTBSensor->setItem(row_count-1,0, pQTableWidgetItemId);
            ui->pTBSensor->setItem(row_count-1,1, pQTableWidgetItemDate);
            ui->pTBSensor->setItem(row_count-1,2, pQTableWidgetItemIllu);

            // 날짜 값을 정한 format으로 읽어옴
            QDateTime xValue = QDateTime::fromString(pQTableWidgetItemDate->text(), "yyyy/MM/dd hh:mm:ss");
            //
            illuLine->append(xValue.toMSecsSinceEpoch(),pQTableWidgetItemIllu->text().toInt());
//            qDebug() << "illu " << pQTableWidgetItemIllu->text().toInt();
        }
        // index를 제일 긴 문자열 기준으로 크기를 맞춤
        ui->pTBSensor->resizeColumnToContents(0);
        ui->pTBSensor->resizeColumnToContents(1);
        ui->pTBSensor->resizeColumnToContents(2);
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
}

Tab5Database::~Tab5Database()
{
    delete ui;
}
