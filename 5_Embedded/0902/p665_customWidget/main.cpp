#include <QApplication>
#include <QPushButton>

#include "CustomWidget.h"

int main (int argc, char **argv){
	QApplication app(argc, argv);		/* QApplication 객체 생성 */
	
	CustomWidget *widget = new CustomWidget(0);
	widget->show();

	return app.exec();					/* Qt의 메인 이벤트 루프 실행 */
}
