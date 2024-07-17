#pragma once
#ifndef EXP_H
#define EXP_H

class Exp {
private:
	int base;
	int exp;

public:
	Exp() { base = 1; exp = 1; }
	Exp(int a) { base = a; exp = 1; }
	Exp(int a, int b) { base = a; exp = b; }

public:
	int getValue(){ return pow(base, exp); }
	int getBase() { return base; }
	int getExp() { return exp; }
	bool equal(Exp);
};

#endif // !EXP_H
