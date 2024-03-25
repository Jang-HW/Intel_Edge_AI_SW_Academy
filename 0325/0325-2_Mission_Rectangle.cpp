#include <iostream>
#include <cstring>
#include <string>

// using std::cout;
// using std::endl;
using namespace std;

// 설계도 제작
class Rectangle
{
	// 멤버변수
	// 클래스의 속성 (성격, 특성)
public:
	int height;
	int width;

	// 멤버함수
	// 클래스의 기능 (행동, 행위)
public:
	Rectangle();
	Rectangle(int);
	Rectangle(int, int);
	double getArea();		// 함수 정의를 여기서 해도 되긴 함. 하지만 권장은 따로 정의. 짧을때는 알아서.
	bool isSquare();
};
Rectangle::Rectangle() {
	height = width = 1;
}
Rectangle::Rectangle(int edge) {
	height = edge;
	width = edge;
}
Rectangle::Rectangle(int h, int w) {
	height = h;
	width = w;
}
double Rectangle::getArea() {
	return height * width;
}
bool Rectangle::isSquare() {
	if (height == width)
		return true;
	else
		return false;
}


int main() {
	Rectangle rect1;
	Rectangle rect2(3, 5);
	Rectangle rect3(3);
	

	if (rect1.isSquare())	cout << "rect1은 정사각형이다." << endl;
	if (rect2.isSquare())	cout << "rect2은 정사각형이다." << endl;
	if (rect3.isSquare())	cout << "rect3은 정사각형이다." << endl;


	return 0;
}