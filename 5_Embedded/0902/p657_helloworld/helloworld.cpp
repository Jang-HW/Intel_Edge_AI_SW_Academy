#include <QApplication>
#include <QLabel>

int main (int argc, char **argv){
	QApplication app(argc, argv);		/* QApplication 객체 생성 */

	QLabel *hello = new QLabel("<font color=blue>Hello <i> World!</i></font>", 0);
	hello->resize(180, 135);
	hello->move(300, 300);
	hello->show();						/* 위젯의 화면 표시 */

	return app.exec();					/* Qt의 메인 이벤트 루프 실행 */
}
