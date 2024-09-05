#include "tab7camsecurity.h"
#include "ui_tab7camsecurity.h"

Tab7CamSecurity::Tab7CamSecurity(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab7CamSecurity)
{
    ui->setupUi(this);
    pCameracapture = new Cameracapture(this);
    // setter 함수를 쓰는게 더 좋은 형식임
    pCameracapture->pTLcamView = ui->pTLcamView;

    // 버튼들의 동작을 위한 함수 지정
    connect(ui->pPBCamView,SIGNAL(clicked(bool)),this, SLOT(slotCamView(bool)));
    connect(ui->pPBsnapShot,SIGNAL(clicked()), this, SLOT(slotSnapShot()));
    connect(ui->pCBsecurityMode,SIGNAL(clicked(bool)), this, SLOT(slotSecurityMode(bool)));
    connect(ui->pCBColorMode,SIGNAL(clicked(bool)), this, SLOT(slotColorMode(bool)));
}

Tab7CamSecurity::~Tab7CamSecurity()
{
    delete ui;
}

// cam on/off 버튼 누름
void Tab7CamSecurity::slotCamView(bool check)
{
    if(check)       //cam on
    {
        // 보안모드 여부 확인
        slotSecurityMode(ui->pCBsecurityMode->isChecked());
        ui->pPBCamView->setText("영상 정지");
        // 카메라가 돌고 안 돌고 있다면
        if(!pCameracapture->getCamFlag())
        {
            pCameracapture->setCamFlag(true);
            pCameracapture->start();
        }
    }
    else
    {
        pCameracapture->camTimerStartStop(false);
        ui->pPBCamView->setText("영상 보기");
        pCameracapture->setCamFlag(false);
    }
}
void Tab7CamSecurity::slotSnapShot()
{
    if(ui->pPBCamView->isChecked())
        pCameracapture->camSnapshot();
}

// 보안 모드의 경우 타이머가 필요함 (1초 간격의 영상의 차이로 확인)
    // 타이머 동작을 제어
void Tab7CamSecurity::slotSecurityMode(bool check)
{
    pCameracapture->camTimerStartStop(check);
}

void Tab7CamSecurity::slotColorMode(bool bCheck)
{
    pCameracapture->colorMode = bCheck;
}

Cameracapture* Tab7CamSecurity::pGetCameracapture()
{
    return pCameracapture;
}
