#include <iostream>
#include <cstring>
#include <string>
using namespace std;


int main() {


	cout << "�� ��? ";
	int n;
	cin >> n;

	// ������ ������ �ʱ�ȭ�Ǵ� ���� �迭
	int* p = new int[n];

	for (int i = 0; i < n; i++) 
		p[i] = 100;
	
	for (int i = 0; i < n; i++)
		cout << p[i] << " ";



	/////////////////////////////////////////



	int height, width;

	cout << "��: ";
	cin >> height;
	cout << "��: ";
	cin >> width;

	// �Է��� ũ���� 2���� �迭 int�� �迭 ������ 127���� �ʱ�ȭ 
	int** array;
	array = new int* [height];

	for (int i = 0; i < height; i++)
		array[i] = new int[width];

	cout << endl;

	for (int i = 0; i < height; i++){
		for (int k = 0; k < width; k++) {
			array[i][k] = 127;

			cout << array[i][k] << " ";
		}
		cout << endl;
	}


	for (int i = 0; i < height; i++)
		delete[] array[i];
		
	delete[] array;

	return 0;
}