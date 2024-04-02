#include <iostream>
#include <cstring>
#include <string>
using namespace std;

class Circle {
private: 
	int radius;

public:
	Circle(int radius) { this->radius = radius; }
	int getRadius() { return this->radius; }
	void setRadius(int radius) { this->radius = radius; }
	
};


void swap(Circle a, Circle b) {
	Circle tmp = 0;
	tmp.setRadius(a.getRadius());
	a.setRadius(b.getRadius());
	b.setRadius(tmp.getRadius());
}

void swap2(Circle *a, Circle *b) {
	Circle tmp = 0;
	tmp.setRadius(a->getRadius());
	a->setRadius(b->getRadius());
	b->setRadius(tmp.getRadius());
}

void swap3(Circle &a, Circle &b) {
	Circle tmp = 0;
	tmp.setRadius(a.getRadius());
	a.setRadius(b.getRadius());
	b.setRadius(tmp.getRadius());
}



int main() {

	Circle c1(1);
	Circle c2(2);

	cout << "호출 전" << endl;
	cout << "c1: " << c1.getRadius() << ", c2: " << c2.getRadius() << endl << endl;

	swap(c1, c2);

	cout << "swap1 호출" << endl;
	cout << "c1: " << c1.getRadius() << ", c2: " << c2.getRadius() << endl;
	cout << endl << "값에 의한 호출" << endl << "함수가 바꾸긴 하는데 값을 복사해서 넘기기 때문에" << endl << "main에서는 안 바뀜" << endl;


	cout << endl << "-------------" << endl << endl;


	cout << "호출 전" << endl;
	cout << "c1: " << c1.getRadius() << ", c2: " << c2.getRadius() << endl << endl;

	swap2(&c1, &c2);

	cout << "swap2 호출" << endl;
	cout << "c1: " << c1.getRadius() << ", c2: " << c2.getRadius() << endl;
	cout << endl << "주소에 의한 호출" << endl << "바뀜!" << endl;



	cout << endl << "-------------" << endl << endl;


	cout << "호출 전" << endl;
	cout << "c1: " << c1.getRadius() << ", c2: " << c2.getRadius() << endl << endl;

	swap3(c1, c2);

	cout << "swap3 호출" << endl;
	cout << "c1: " << c1.getRadius() << ", c2: " << c2.getRadius() << endl;
	cout << endl << "참조에 의한 호출" << endl << "바뀜!" << endl;


	return 0;
}