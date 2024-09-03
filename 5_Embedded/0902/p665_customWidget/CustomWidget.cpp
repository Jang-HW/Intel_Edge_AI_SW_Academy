#include <QApplication>
#include <QPushButton>
#include <QLabel>

#include "CustomWidget.h"

CustomWidget::CustomWidget(QWidget *parent) : QWidget(parent) {

	QLabel *hello = new QLabel("<font color=blue>Hello <i> World!</i></font>", this);
    hello->resize(180, 35);


	QPushButton *button = new QPushButton("Quit", this);
	button->resize(120, 35);
	button->move(0, 35);

	this->resize(240, 70);
	this->move(300, 300);

	QObject::connect(button, SIGNAL(clicked()), qApp, SLOT(quit()));
}
