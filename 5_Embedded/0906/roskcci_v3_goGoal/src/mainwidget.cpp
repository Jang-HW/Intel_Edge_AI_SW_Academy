#include "../include/roskcci/mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(int argc, char** argv, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    setWindowTitle("KCCI AIOT ROS");
    pTab1RosControl = new Tab1RosControl(argc, argv, ui->pTab1);
    ui->pTab1->setLayout(pTab1RosControl->layout());
}

MainWidget::~MainWidget()
{
    delete ui;
}

