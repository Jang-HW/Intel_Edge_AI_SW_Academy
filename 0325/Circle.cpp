#include <iostream>
#include <cstring>
#include <string>
using namespace std;

#include "Circle.h"


Circle::Circle() { }

Circle::Circle(int r) {
	cout << "�� �ν��Ͻ� ����" << endl;
	radius = r;
}

Circle::~Circle() {
	cout << "���ڿ�~" << endl;
}

double Circle::getArea() {
	return radius * radius * 3.14;
}