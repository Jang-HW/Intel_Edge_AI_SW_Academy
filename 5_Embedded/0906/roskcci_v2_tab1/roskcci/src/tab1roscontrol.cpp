#include "../include/roskcci/tab1roscontrol.h"
#include "ui_tab1roscontrol.h"
Tab1RosControl::Tab1RosControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab1RosControl)
{
    ui->setupUi(this);
}

Tab1RosControl::~Tab1RosControl()
{
    delete ui;
}
