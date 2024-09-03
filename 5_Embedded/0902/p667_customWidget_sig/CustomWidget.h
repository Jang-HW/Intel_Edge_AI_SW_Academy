#ifndef __CUTSOM_WIDGET__
#define __CUTSOM_WIDGET__

#include <QWidget>

class CustomWidget : public QWidget {
		Q_OBJECT 
		public :
			CustomWidget(QWidget *parent = 0);
		signals:
			void widgetClicked();
		public slots:
			void processClick();
};

#endif // __CUSTOM_WIDGET__
