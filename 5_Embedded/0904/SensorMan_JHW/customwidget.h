#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>
#include "tab2sockeclient.h"
#include "tab4chartplot.h"
#include "tab5database.h"


QT_BEGIN_NAMESPACE
namespace Ui { class CustomWidget; }
QT_END_NAMESPACE

class CustomWidget : public QWidget
{
    Q_OBJECT

public:
    CustomWidget(QWidget *parent = nullptr);
    ~CustomWidget();

private:
    Ui::CustomWidget *ui;
    Tab2SockeClient *pTab2SocketClient;
    Tab4ChartPlot *pTab4ChartPlot;
    Tab5Database *pTab5Database;
};
#endif // CUSTOMWIDGET_H
