#ifndef TAB1ROSCONTROL_H
#define TAB1ROSCONTROL_H

#include <QWidget>

namespace Ui {
class Tab1RosControl;
}

class Tab1RosControl : public QWidget
{
    Q_OBJECT

public:
    explicit Tab1RosControl(QWidget *parent = nullptr);
    ~Tab1RosControl();

private:
    Ui::Tab1RosControl *ui;
};

#endif // TAB1ROSCONTROL_H
