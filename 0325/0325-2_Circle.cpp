#include <iostream>
#include <cstring>
#include <string>

// using std::cout;
// using std::endl;
using namespace std;

// ���� ���赵 ����
class Circle
{
// �������  >  Ŭ������ �Ӽ� (����, Ư��)
private:
	int radius;				// ������
	int myvar;

// Getter, Setter
public:		
	inline void	setRadius(int value)	{ radius = value; }
	inline void	setMyvar(int value)		{ myvar = value; }
	inline int	getRadius()				{ return radius; }
	inline int	getMyvar()				{ return myvar; }


// ����Լ�  >  Ŭ������ ��� (�ൿ, ����)
public:
	Circle();
	Circle(int);
	~Circle();
	double getArea();		// �Լ� ���Ǹ� ���⼭ �ص� �Ǳ� ��. ������ ������ ���� ����. ª������ �˾Ƽ�. 
};
Circle::Circle() {
}
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



int main() {
	Circle dounutA(1);
	Circle dounutB;
	dounutB.setRadius(2);

	cout << "����A�� ������ " << dounutA.getArea() << "�Դϴ�." << endl;
	cout << "����B�� ������ " << dounutB.getArea() << "�Դϴ�." << endl;

	return 0;
}