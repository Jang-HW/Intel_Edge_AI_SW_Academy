#ifndef TAB7CAMSECURITY_H
#define TAB7CAMSECURITY_H

#include <QWidget>
#include "cameracapture.h"
namespace Ui {
class Tab7CamSecurity;
}

class Tab7CamSecurity : public QWidget
{
    Q_OBJECT

public:
    explicit Tab7CamSecurity(QWidget *parent = nullptr);
    ~Tab7CamSecurity();
    Cameracapture* pGetCameracapture();
private:
    Ui::Tab7CamSecurity *ui;
    Cameracapture * pCameracapture;
private slots:
    void slotCamView(bool);
    void slotSnapShot();
    void slotSecurityMode(bool);
    void slotColorMode(bool);
};

#endif // TAB7CAMSECURITY_H
