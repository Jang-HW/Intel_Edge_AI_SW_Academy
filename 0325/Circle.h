#pragma once
#ifndef CIRCLE_H
#define CIRCLE_H

class Circle
{
	// 멤버변수  >  클래스의 속성 (성격, 특성)
private:
	int radius;				// 반지름
	int myvar;

	// Getter, Setter
public:
	inline void	setRadius(int value) { radius = value; }
	inline void	setMyvar(int value) { myvar = value; }
	inline int	getRadius() { return radius; }
	inline int	getMyvar() { return myvar; }


	// 멤버함수  >  클래스의 기능 (행동, 행위)
public:
	Circle();
	Circle(int);
	~Circle();
	double getArea();		// 함수 정의를 여기서 해도 되긴 함. 하지만 권장은 따로 정의. 짧을때는 알아서. 
};

#endif