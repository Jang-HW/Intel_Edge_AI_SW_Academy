#ifndef TAB1ROSCONTROL_H
#define TAB1ROSCONTROL_H

#include <QWidget>
#include <QButtonGroup>
#include <QPushButton>
#include <iostream>
#include <array>
#include "rosnode.h"

namespace Ui {
class Tab1RosControl;
}

class Tab1RosControl : public QWidget
{
    Q_OBJECT

public:
    explicit Tab1RosControl(int argc, char **argv, QWidget *parent = nullptr);
    ~Tab1RosControl();

private:
    Ui::Tab1RosControl *ui;
    RosNode *prosNode;
    QButtonGroup *bg;
    QWidget *widget;

private slots:
    void goal_Pub();
    void goal_PubSlot(double, double, double, double);
    void goal_Pub_Front();
    void goal_Pub_Living();
    void goal_Pub_Study();
    void goal_Pub_Bedroom();
    void set_Pub();
    void slotLdsReceive(float *);
};

#endif // TAB1ROSCONTROL_H
