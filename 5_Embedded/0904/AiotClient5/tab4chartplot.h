#ifndef TAB4CHARTPLOT_H
#define TAB4CHARTPLOT_H

#include <QWidget>
#include <QLineSeries>
#include <QChartView>
#include <QDebug>
#include <QDateTimeAxis>
#include <QDate>
#include <QTime>
QT_CHARTS_USE_NAMESPACE
namespace Ui {
class Tab4ChartPlot;
}

class Tab4ChartPlot : public QWidget
{
    Q_OBJECT

public:
    explicit Tab4ChartPlot(QWidget *parent = nullptr);
    ~Tab4ChartPlot();

private:
    Ui::Tab4ChartPlot *ui;
    QLineSeries* illuLine;
    QChart *chart = nullptr;
    QChartView *chartView = nullptr;
    // 시간을 축으로 사용하기 위한 변수
    QDateTimeAxis * axisX;
    // 시작 시간과 종료 시간이 있어야지 축을 그릴 수 있음
    QDateTime firstDate;
    QDateTime lastDate;
private slots:
    void chartPlotClear();
    // tab 2에서 들어온 데이터 값을 받기 위한 슬롯
    void tab4RecvDataSlot(QString);
    // x축이 기본 10분인데, 창에 꽉차면 늘려주기 위한 슬롯
    void updateLastDateTime(int);
};

#endif // TAB4CHARTPLOT_H

