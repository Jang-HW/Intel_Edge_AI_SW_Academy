#include "cameracapture.h"
int Cameracapture::RED=1;
int Cameracapture::GREEN=2;
int Cameracapture::BLUE=3;
Cameracapture::Cameracapture(QWidget *parent) :  QThread(parent)
{
    camFlag = false;        // while문의 동작을 제어하는 변수
    saveFlag = false;       // 보안모드에서 1초 전의 사진 저장 여부
    timerFlag = false;      // 보안모드의 작동 여부
    intruderFlag = false;   // 경보가 1초간만 울리도록 하기 위한 변수
    colorMode = false;      // 색상을 보내는 기능을 동작 여부
    frameCount = 0;         // 촬영한 프레임 수를 저장하기 위한 변수
    mColor = 0;             // 컬러모드의 이전 색상을 저장하기 위한 변수
    waitTime = 1000; //1000ms
    pQTimer = new QTimer(this);
    connect(pQTimer, SIGNAL(timeout()),this,SLOT(slotMScounter()));
}

// time out의 동작 정의
void Cameracapture::slotMScounter() {
    // 플래그만 참으로
    timerFlag=true;
}

// 카메라의 타이머 정지
void Cameracapture::camTimerStartStop(bool check) {
    if(check)
       pQTimer->start(waitTime);
    else if(pQTimer->isActive())
       pQTimer->stop();
}
void Cameracapture::run()
{
    QString text;
    VideoCapture  capture(0);
    if (!capture.isOpened())
    {
        cout << "카메라가 연결되지 않았습니다." << endl;
        exit(1);
    }

    // 카메라 실행여부 확인
    // 이거 때문에 thread 상속 받은거
    while(camFlag) {
        capture.read(frame);
        if(colorMode)
            colorProcess();
        put_string(frame, "count : ", Point(10, 40), frameCount++);
        // 프레임을 cv가 쓰는 bgr에서 qt가 쓰는 rgb로 변경
        cvtColor(frame, frameQt, COLOR_BGR2RGB);
        // qt가 쓰는 이미지 포멧으로 변경
        QImage _img(frameQt.data, frameQt.cols, frameQt.rows, QImage::Format_RGB888);
        // 중간에 에임 그리는 거
        line(frameQt, Point((frameQt.cols >> 1) - 15, frameQt.rows >> 1), Point((frameQt.cols >> 1) + 15, frameQt.rows >> 1), Scalar(255, 0, 0), 2);
        line(frameQt, Point(frame.cols >> 1, (frameQt.rows >> 1) - 15), Point(frameQt.cols >> 1, (frameQt.rows >> 1) + 15), Scalar(255, 0, 0), 2);
        // 이미지를 뿌림
        pTLcamView->setPixmap(QPixmap::fromImage(_img));

        // 보안모드 확인
        if(timerFlag) {
             securityProcess();
             timerFlag = false;
        }

    }
    capture.release();
//    destroyWindow("카메라 영상보기");
}

// 이미지 위에 캡쳐한 갯수를 쓰기 위한 함수
void Cameracapture::put_string(Mat &frame, string text, Point pt, uint32_t value)
{
    text += to_string(value);
    Point shade = pt + Point(2, 2);
    int font = FONT_HERSHEY_SIMPLEX;
    putText(frame, text, shade, font, 0.7, Scalar(0, 0, 0), 2);     // 그림자 효과
    putText(frame, text, pt, font, 0.7, Scalar(120, 200, 90), 2);// 작성 문자
}

// 캠 플래그 게터
bool Cameracapture::getCamFlag()
{
    return camFlag;
}

// 캠 플레그 setter
void Cameracapture::setCamFlag(bool camFlag)
{
    this->camFlag = camFlag;
}

// 이미지 저장
void Cameracapture::camSnapshot()
{
//    cvtColor(frame, frame, COLOR_RGB2BGR);
    imwrite("capture.jpg", frame);
}

// 보안모드일때, 동작하는 함수
void Cameracapture::securityProcess()
{
    int pixelCount = 0;
    // 이미지가 저장이 안되 있다면
    if(!saveFlag)
    {
        // 일단 저장함
        // deep copy
        securityFrame = frameQt.clone();
        saveFlag = true;
        // 침입경보 송신 여부
        if(intruderFlag)
        {
            emit sigSocketSendData("[HM_CON]GASOFF");
            intruderFlag = false;
        }
    }
    // 이미지가 저장되어 있으면, 기존 것과 지금 것을 비교
    else
    {
        securityFrame = abs(securityFrame - frameQt);
        // 이미지 프료세싱을 위한 포인터로 주소 저장 (포인터가 빠름)
        uchar* my_ptr = (uchar*)securityFrame.data; // v+1행 첫 칸의 주소를 불러온다.

        for(int v= 0;v<securityFrame.rows ; v++){
            for(int u = 0 ; u<securityFrame.cols; u++){
                // 픽셀 값이 30 이상 차이 나면 count를 증가
                if(my_ptr[v*securityFrame.cols + u] >=30) pixelCount++;
                    // 일정이상 차이 나면 바로 나가도록 하는 코드
                    // 여기 주석을 풀면 밑에 쪽도 풀어야 됨
//                   if(pixelCount > 3072)  //1%
 //                      break;
            }
//          if(pixelCount > 3072)
//              break;
        }
        // 보안 모드에서 차이나는 픽셀 숫자의 기준
        if(pixelCount > 3072)
        {
            // 보안 작동
//            cvtColor(frame, frame, COLOR_RGB2BGR);
            imwrite("security.jpg", frame);
            emit sigSocketSendData("[HM_CON]INTRUDER");
            QThread::msleep(200);
            emit sigSocketSendData("[HM_CON]GASON");
            qDebug() << "INTRUDER " << pixelCount ;
            intruderFlag = true;
         }
        // 저장한 사진은 이미 썻으니까 flag off
         saveFlag = false;
    }
}
void Cameracapture::colorProcess()
{
    Mat img_hsv;
    // HSV로 이미지를 변경
        // 색상처리에 좋은 성능
    cvtColor(frame, img_hsv, COLOR_BGR2HSV);

    uchar* pointer_row = img_hsv.ptr<uchar>(img_hsv.rows >> 1);

    uchar h = pointer_row[(img_hsv.cols >> 1) * 3 + 0];
    uchar s = pointer_row[(img_hsv.cols >> 1) * 3 + 1];
    uchar v = pointer_row[(img_hsv.cols >> 1) * 3 + 2];

    // 조명에 따라서 색상의 범위가 달라짐
    // 색상이 기존과 달라졌을 때만 전송하기 위해서 if문에서 함께 검사
    if((mColor != Cameracapture::RED) && (0 <= h && h <= 10))            //red
    {
        emit sigSocketSendData("[KSH_LIN]COLOR@RED");
        mColor = Cameracapture::RED;
        qDebug() << " h:" << h << " s:" << s <<" v:" <<v << endl;
    }
    else if((mColor != Cameracapture::GREEN) && (50 <= h && h <= 70))         //green
    {
        emit sigSocketSendData("[KSH_LIN]COLOR@GREEN");
        mColor = Cameracapture::GREEN;
        qDebug() << " h:" << h << " s:" << s <<" v:" <<v << endl;
    }
    else if((mColor != Cameracapture::BLUE) && (100 <= h && h <= 120))       //blue
    {
        emit sigSocketSendData("[KSH_LIN]COLOR@BLUE");
        mColor = Cameracapture::BLUE;
        qDebug() << " h:" << h << " s:" << s <<" v:" <<v << endl;
    }
}
