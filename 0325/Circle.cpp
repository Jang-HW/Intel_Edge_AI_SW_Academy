#include <iostream>
#include <cstring>
#include <string>
using namespace std;

#include "Circle.h"


Circle::Circle() { }

Circle::Circle(int r) {
	cout << "새 인스턴스 생성" << endl;
	radius = r;
}

Circle::~Circle() {
	cout << "잘자요~" << endl;
}

double Circle::getArea() {
	return radius * radius * 3.14;
}