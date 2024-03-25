#include <iostream>
using namespace std;

#include "Exp.h"

bool Exp::equal(Exp val) {
	if (pow(base, exp) == val.getValue())
		return true;
	else
		return false;
}