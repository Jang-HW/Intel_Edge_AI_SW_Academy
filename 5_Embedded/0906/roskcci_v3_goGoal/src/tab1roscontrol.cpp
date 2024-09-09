#include "../include/roskcci/tab1roscontrol.h"
#include "ui_tab1roscontrol.h"
Tab1RosControl::Tab1RosControl(int argc, char **argv, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab1RosControl)
{
    ui->setupUi(this);
    prosNode = new RosNode(argc, argv);
    prosNode->pLcamView = ui->pLcamView;
    prosNode->start();

    connect(ui->pushButtonGo, SIGNAL(clicked()),this, SLOT(goal_Pub()));
    connect(ui->pPBFront, SIGNAL(clicked()),this, SLOT(goal_Pub_Front()));
    connect(ui->pPBLiving, SIGNAL(clicked()),this, SLOT(goal_Pub_Living()));
    connect(ui->pPBStudy, SIGNAL(clicked()),this, SLOT(goal_Pub_Study()));
    connect(ui->pPBedroom, SIGNAL(clicked()),this, SLOT(goal_Pub_Bedroom()));
    connect(prosNode, SIGNAL(sigLdsReceive(float *)),this, SLOT(slotLdsReceive(float *)));
    connect(prosNode, SIGNAL(rosShutdown()),qApp, SLOT(quit()));

}

Tab1RosControl::~Tab1RosControl()
{
    delete ui;
}

void Tab1RosControl::goal_Pub()
{
    prosNode->go_goal("map",ui->doubleSpinBox1->value(),ui->doubleSpinBox2->value(), ui->doubleSpinBox3->value(),ui->doubleSpinBox4->value());

}
void Tab1RosControl::goal_PubSlot(double, double, double, double)
{

}
void Tab1RosControl::goal_Pub_Front()
{

}
void Tab1RosControl::goal_Pub_Living()
{

}
void Tab1RosControl::goal_Pub_Study(){

}
void Tab1RosControl::goal_Pub_Bedroom(){

}
void Tab1RosControl::set_Pub()
{

}
void Tab1RosControl::slotLdsReceive(float *pscanData)
{
    ui->lcdNumber1->display(pscanData[0]);
    ui->lcdNumber2->display(pscanData[1]);
    ui->lcdNumber3->display(pscanData[2]);
    ui->lcdNumber4->display(pscanData[3]);
}
