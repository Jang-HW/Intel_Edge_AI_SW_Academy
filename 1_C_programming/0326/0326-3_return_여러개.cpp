#include <iostream>
#include <cstring>
#include <string>
using namespace std;

// ����� ���ϴ� �Լ�
// �Է�: a �迭, a �迭 ũ��, �־ ������ avg
// ���: ���� ����
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
		cout << "1�� �̻�" << endl;

	return 0;
}