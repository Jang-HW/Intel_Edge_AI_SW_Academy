#ifndef TAB1DEVCONTROL_H
#define TAB1DEVCONTROL_H

#include <QWidget>
#include <QTimer>
#include <QDial>
#include <QButtonGroup>
#include "keyled.h"

namespace Ui {
class Tab1DevControl;
}

class Tab1DevControl : public QWidget
{
    Q_OBJECT

public:
    explicit Tab1DevControl(QWidget *parent = nullptr);
    QDial *getpDial();
    ~Tab1DevControl();

private:
    Ui::Tab1DevControl *ui;
    KeyLed *pKeyLed;

    // 타이머의 값을 받아오기 위한 개체
    QTimer *pQTimer;

    // 버튼과 check 박스 값 변경에 쓰는 변수
    int lcdData;

    // 체크 박스를 묶기 위한 버튼 그룹 변수
    QButtonGroup *pQButtonGroup;
    QCheckBox *pQCheckBoxArray[8];

private slots:
    void on_pPBquit_clicked();
    void slotTimerStartButton(bool);
    void slotDialSetValue();
    void slotKeyCheckBoxUpdate(int);
    void slotKeyCheckBoxUpdateMouse(int);
    void on_pCBtimerValue_currentIndexChanged(const QString &arg1);

signals:
    void writeLedDataSig(int);
};

#endif // TAB1DEVCONTROL_H
