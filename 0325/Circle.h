#pragma once
#ifndef CIRCLE_H
#define CIRCLE_H

class Circle
{
	// �������  >  Ŭ������ �Ӽ� (����, Ư��)
private:
	int radius;				// ������
	int myvar;

	// Getter, Setter
public:
	inline void	setRadius(int value) { radius = value; }
	inline void	setMyvar(int value) { myvar = value; }
	inline int	getRadius() { return radius; }
	inline int	getMyvar() { return myvar; }


	// ����Լ�  >  Ŭ������ ��� (�ൿ, ����)
public:
	Circle();
	Circle(int);
	~Circle();
	double getArea();		// �Լ� ���Ǹ� ���⼭ �ص� �Ǳ� ��. ������ ������ ���� ����. ª������ �˾Ƽ�. 
};

#endif