#include <iostream>
#include <cstring>
#include <string>
using namespace std;

#include "Circle.h"


int main() {
	Circle dounutA(1);
	Circle dounutB;
	dounutB.setRadius(2);

	cout << "����A�� ������ " << dounutA.getArea() << "�Դϴ�." << endl;
	cout << "����B�� ������ " << dounutB.getArea() << "�Դϴ�." << endl;
	cout << endl;

	Circle dounut;
	dounut.setRadius(30);
	cout << "dounut.getArea()" << endl << " - " << dounut.getArea() << endl;

	Circle* p2;
	p2 = &dounut;
	p2->setRadius(50);
	cout << "p2->getArea()" << endl << " - " << p2->getArea() << endl;

	cout << endl;

	return 0;
}