#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QObject::connect(ui->button, SIGNAL(clicked()), this, SLOT(processClick()));
    QObject::connect(this, SIGNAL(widgetClicked()), qApp, SLOT(quit()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::processClick()
{
//  emit widgetClicked();
//  qApp->quit();

    ui->hello->setText("Hi");
}
