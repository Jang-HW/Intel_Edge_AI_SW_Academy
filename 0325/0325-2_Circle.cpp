#include <iostream>
#include <cstring>
#include <string>

// using std::cout;
// using std::endl;
using namespace std;

// 원의 설계도 제작
class Circle
{
// 멤버변수  >  클래스의 속성 (성격, 특성)
private:
	int radius;				// 반지름
	int myvar;

// Getter, Setter
public:		
	inline void	setRadius(int value)	{ radius = value; }
	inline void	setMyvar(int value)		{ myvar = value; }
	inline int	getRadius()				{ return radius; }
	inline int	getMyvar()				{ return myvar; }


// 멤버함수  >  클래스의 기능 (행동, 행위)
public:
	Circle();
	Circle(int);
	~Circle();
	double getArea();		// 함수 정의를 여기서 해도 되긴 함. 하지만 권장은 따로 정의. 짧을때는 알아서. 
};
Circle::Circle() {
}
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



int main() {
	Circle dounutA(1);
	Circle dounutB;
	dounutB.setRadius(2);

	cout << "도넛A의 면적은 " << dounutA.getArea() << "입니다." << endl;
	cout << "도넛B의 면적은 " << dounutB.getArea() << "입니다." << endl;

	return 0;
}