#include "tab6webcamera.h"
#include "ui_tab6webcamera.h"

Tab6WebCamera::Tab6WebCamera(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab6WebCamera)
{
    ui->setupUi(this);

    pQProcess = new QProcess(this);
    pQWebEngineView = new QWebEngineView(this);
//    pQWebEngineView->load(QUrl(QStringLiteral("http://10.10.141.51:8080/?action=stream")));
//    ui->pGPView->setLayout((pQWebEngineView->layout()));

    connect(ui->pPBCamStart, SIGNAL(clicked(bool)), this, SLOT(camStartSlot(bool)));
}

Tab6WebCamera::~Tab6WebCamera()
{
    delete ui;
}

void Tab6WebCamera::camStartSlot(bool bCheck){
    QString program = "/home/ubuntu/mjpg-streamer-master/mjpg-streamer-experimental/mjpg_streamer -i \"/home/ubuntu/mjpg-streamer-master/mjpg-streamer-experimental/input_uvc.so\" -o \"/home/ubuntu/mjpg-streamer-master/mjpg-streamer-experimental/output_http.so -w /home/ubuntu/mjpg-streamer-master/mjpg-streamer-experimental/www\"";
    // 실행파일의 경우, 상대경로로 만들어지면 안되기 때문에 절대경로로 지정 필요
    if(bCheck){
        pQProcess->start(program);

        if(pQProcess->waitForStarted()){
            // 시작 직후에 연결을 하면 오류가 나서 약간 지연
            QThread::msleep(500);
            pQWebEngineView->load(QUrl(QStringLiteral("http://10.10.141.51:8080/?action=stream")));
            ui->pGPView->setLayout((pQWebEngineView->layout()));
            ui->pPBCamStart->setText("CamStop");
        }
    }
    else {
        pQProcess->kill();
        pQWebEngineView->stop();
        ui->pPBCamStart->setText("CamStart");
    }
}
