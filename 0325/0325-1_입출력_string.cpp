#include <iostream>
#include <cstring>
#include <string>

// using std::cout;
// using std::endl;
using namespace std;

int main() {

	cout << "Hello world!\n";			// 그자리에서 처리해 빠르고 버그 ▼
	printf("%s", "Hello woeld!\n");			// 함수를 들고와서 느리고 버그가 多

	cout << endl << endl << endl;

	cout << "너비: ";
	int width;
	cin >> width;
	cout << "입력한 너비는 " << width << "입니다. " << endl;

	cout << endl << endl << endl;

	char password[11];
	cout << "암호: ";
	cin >> password;
	cout << "입력한 암호는 \"" << password << "\"입니다. " << endl;
	cout << "(띄어쓰기시 짤림)" << endl << endl;

	char passwordTwo[11];
	cout << "암호: ";
	cin.getline(passwordTwo, 11, '\n');
	cout << "입력한 암호는 \"" << passwordTwo << "\"입니다. " << endl;
	cout << "(띄어쓰기시 안 짤림)" << endl;
	cout << "앞에 거에서 입력한 띄어쓰기가 입력 버퍼에서 이쪽으로 입력되기도" << endl;

	cout << endl << endl << endl;

	string song("안녕하세요, 방가방가");
	song = string("에르비스 프레스르리");
	string singer("엘비스 프래슬리");
	singer = "뭐가 대표곡인지 몰라잉";

	cout << "노래불러! " << song << endl << "누가? " << singer << endl;





	return 0;
}