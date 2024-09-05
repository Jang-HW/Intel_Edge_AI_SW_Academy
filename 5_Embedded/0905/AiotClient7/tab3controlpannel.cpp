#include "tab3controlpannel.h"
#include "ui_tab3controlpannel.h"

Tab3ControlPannel::Tab3ControlPannel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab3ControlPannel)
{
    ui->setupUi(this);
    paletteColorOn.setColor(ui->pPBLamp->backgroundRole(), QColor(255, 0, 0));
    paletteColorOff.setColor(ui->pPBLamp->backgroundRole(), QColor(0, 0, 255));
    ui->pPBLamp->setPalette(paletteColorOff);
    ui->pPBplug->setPalette(paletteColorOff);
}

Tab3ControlPannel::~Tab3ControlPannel()
{
    delete ui;
}

// 버튼 눌렀을 때, tab2에 전송하도록 하는 명령어
void Tab3ControlPannel::on_pPBLamp_clicked(bool checked)
{
    if(checked) {
        emit socketSendDataSig("[HM_CON]LAMPON");
        ui->pPBLamp->setChecked(true);
        ui->pPBLamp->setIcon(QIcon(":/Image/Image/light_off.png"));
    }
    else{
        emit socketSendDataSig("[HM_CON]LAMPOFF");
        ui->pPBLamp->setChecked(false);
        ui->pPBLamp->setIcon(QIcon(":/Image/Image/light_on.png"));
    }
}

void Tab3ControlPannel::on_pPBplug_clicked(bool checked)
{
    if(checked) {
        emit socketSendDataSig("[HM_CON]GASON");
        ui->pPBplug->setChecked(true);
        ui->pPBplug->setIcon(QIcon(":/Image/Image/plug_off.png"));
    }
    else{
        emit socketSendDataSig("[HM_CON]GASOFF");
        ui->pPBplug->setChecked(false);
        ui->pPBplug->setIcon(QIcon(":/Image/Image/plug_on.png"));
    }
}

void Tab3ControlPannel::tab3RecvDataSlot(QString recvData){
//    qDebug() << recvData;
    QStringList qList = recvData.split("@");
    if(qList[2] == "GASON") {
        ui->pPBplug->setIcon(QIcon(":/Image/Image/plug_on.png"));
        ui->pPBplug->setPalette(paletteColorOn);
    }
    else if(qList[2] == "GASOFF"){
        ui->pPBplug->setIcon(QIcon(":/Image/Image/plug_off.png"));
        ui->pPBplug->setPalette(paletteColorOff);
    }
    else if(qList[2] == "LAMPON") {
        ui->pPBLamp->setIcon(QIcon(":/Image/Image/light_on.png"));
        ui->pPBLamp->setPalette(paletteColorOn);
    }
    else if(qList[2] == "LAMPOFF"){
        ui->pPBLamp->setIcon(QIcon(":/Image/Image/light_off.png"));
        ui->pPBLamp->setPalette(paletteColorOff);
    }
}
