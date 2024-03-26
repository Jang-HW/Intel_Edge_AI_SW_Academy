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

	cout << "ȣ�� ��" << endl;
	cout << "c1: " << c1.getRadius() << ", c2: " << c2.getRadius() << endl << endl;

	swap(c1, c2);

	cout << "swap1 ȣ��" << endl;
	cout << "c1: " << c1.getRadius() << ", c2: " << c2.getRadius() << endl;
	cout << endl << "���� ���� ȣ��" << endl << "�Լ��� �ٲٱ� �ϴµ� ���� �����ؼ� �ѱ�� ������" << endl << "main������ �� �ٲ�" << endl;


	cout << endl << "-------------" << endl << endl;


	cout << "ȣ�� ��" << endl;
	cout << "c1: " << c1.getRadius() << ", c2: " << c2.getRadius() << endl << endl;

	swap2(&c1, &c2);

	cout << "swap2 ȣ��" << endl;
	cout << "c1: " << c1.getRadius() << ", c2: " << c2.getRadius() << endl;
	cout << endl << "�ּҿ� ���� ȣ��" << endl << "�ٲ�!" << endl;



	cout << endl << "-------------" << endl << endl;


	cout << "ȣ�� ��" << endl;
	cout << "c1: " << c1.getRadius() << ", c2: " << c2.getRadius() << endl << endl;

	swap3(c1, c2);

	cout << "swap3 ȣ��" << endl;
	cout << "c1: " << c1.getRadius() << ", c2: " << c2.getRadius() << endl;
	cout << endl << "������ ���� ȣ��" << endl << "�ٲ�!" << endl;


	return 0;
}