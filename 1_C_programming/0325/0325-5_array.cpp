#include <iostream>
#include <cstring>
#include <string>
using namespace std;

#include "Circle.h"

int main() {
	
	Circle circleArray[3];

	circleArray[0].setRadius(10);
	circleArray[1].setRadius(20);
	circleArray[2].setRadius(30);

	for (int i = 0; i < 3; i++)
		cout << circleArray[i].getArea() << endl;

	cout << endl;

	Circle* p;
	p = circleArray;
	for (int i = 0; i < 3; i++) {
		cout << circleArray[i].getArea() << endl;
		// cout << p->getArea() << endl;
		// p++;
	}

	cout << endl;

	return 0;
}