#include <QApplication>
#include <QPushButton>

int main (int argc, char **argv){
	QApplication app(argc, argv);		/* QApplication 객체 생성 */

	QPushButton *button = new QPushButton("Quit", 0);
	button->resize(120, 35);
	button->move(300, 300);
	button->show();						/* 위젯의 화면 표시 */

	// 664p signal-slot
	QObject::connect(button, SIGNAL(clicked()), &app, SLOT(quit()));
	return app.exec();					/* Qt의 메인 이벤트 루프 실행 */
}
