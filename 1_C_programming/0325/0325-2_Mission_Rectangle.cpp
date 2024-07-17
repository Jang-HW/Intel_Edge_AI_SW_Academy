#include <iostream>
#include <cstring>
#include <string>

// using std::cout;
// using std::endl;
using namespace std;

// ���赵 ����
class Rectangle
{
	// �������
	// Ŭ������ �Ӽ� (����, Ư��)
public:
	int height;
	int width;

	// ����Լ�
	// Ŭ������ ��� (�ൿ, ����)
public:
	Rectangle();
	Rectangle(int);
	Rectangle(int, int);
	double getArea();		// �Լ� ���Ǹ� ���⼭ �ص� �Ǳ� ��. ������ ������ ���� ����. ª������ �˾Ƽ�.
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
	

	if (rect1.isSquare())	cout << "rect1�� ���簢���̴�." << endl;
	if (rect2.isSquare())	cout << "rect2�� ���簢���̴�." << endl;
	if (rect3.isSquare())	cout << "rect3�� ���簢���̴�." << endl;


	return 0;
}