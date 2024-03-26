#include <iostream>
#include <cstring>
#include <string>
using namespace std;

// 평균을 구하는 함수
// 입력: a 배열, a 배열 크기, 넣어서 돌려줄 avg
// 출력: 성공 여부
int avergage(int a[], int size, double& avg) {
	if (size <= 0)
		return false;
	
	double sum = 0;
	for (int i = 0; i < size; i++) 
		sum += a[i];
	
	avg = sum / size;
	return true;
}


int main() {

	int x[] = { 1, 2, 3, 4 };
	double avg = 0;
	if (avergage(x, 4, avg))
		cout << avg << endl;
	else
		cout << "1개 이상" << endl;

	return 0;
}