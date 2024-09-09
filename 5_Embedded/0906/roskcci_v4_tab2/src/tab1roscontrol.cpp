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
void Tab1RosControl::goal_PubSlot(double x, double y, double z, double w)
{
   prosNode->go_goal("map",x,y,z,w);
}
void Tab1RosControl::goal_Pub_Front()
{
   prosNode->go_goal("map",0.83, 2.92, 0.0, 0.72);
}
void Tab1RosControl::goal_Pub_Living()
{
   prosNode->go_goal("map",0.077, 0.19, 0.0, 0.79);
}
void Tab1RosControl::goal_Pub_Study()
{
   prosNode->go_goal("map",4.96, 6.366, 0.0, 0.62);
}
void Tab1RosControl::goal_Pub_Bedroom()
{
   prosNode->go_goal("map",7.78, 1.05, 0.0, 0.59);
}
void Tab1RosControl::set_Pub()
{
   prosNode->set_goal("map",0.83, 2.92, 0.0, 0.72);
}
void Tab1RosControl::slotLdsReceive(float *pscanData)
{
    ui->lcdNumber1->display(pscanData[0]);
    ui->lcdNumber2->display(pscanData[1]);
    ui->lcdNumber3->display(pscanData[2]);
    ui->lcdNumber4->display(pscanData[3]);
}
