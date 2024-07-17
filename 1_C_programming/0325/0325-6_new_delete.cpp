#include <iostream>
#include <cstring>
#include <string>
using namespace std;


int main() {


	cout << "몇 개? ";
	int n;
	cin >> n;

	// 변수로 갯수가 초기화되는 동적 배열
	int* p = new int[n];

	for (int i = 0; i < n; i++) 
		p[i] = 100;
	
	for (int i = 0; i < n; i++)
		cout << p[i] << " ";



	/////////////////////////////////////////



	int height, width;

	cout << "행: ";
	cin >> height;
	cout << "열: ";
	cin >> width;

	// 입력한 크기의 2차원 배열 int형 배열 생성후 127으로 초기화 
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