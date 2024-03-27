#include <iostream>
#include <cstring>
#include <string>
using namespace std;

class Circle {
	/// 클래스의 속성(=성격=특성)
private :
	int radius;
	static int numOfCircles;

	/// 클래스의 기능(=행동=행위)
public : // 게터/세터
	Circle(int);
	~Circle() { numOfCircles--; }
	void setRadius(int radius) { this->radius = radius; };
	int getRadius() { return radius; };
	static int getNumOfCircles() { return numOfCircles; }

public :
	double getArea() ;
};
Circle::Circle(int radius = 1) { 
	this->radius = radius;
	numOfCircles++;
}
double Circle::getArea() {
	return radius * radius * 3.14;
}

int Circle::numOfCircles = 0;


int main() {
	Circle* p = new Circle[10];
	cout << "현재 원 객체수: " << Circle::getNumOfCircles() << endl;

	delete[] p;
	cout << "원의 수: " << Circle::getNumOfCircles() << endl;

	if (true) {
		Circle a, b;
		cout << "원의 수 : " << Circle::getNumOfCircles() << endl;
	}

	cout << "원의 수: " << Circle::getNumOfCircles() << endl;

	return 0;

}