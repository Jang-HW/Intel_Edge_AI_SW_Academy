#include <iostream>
#include <cstring>
#include <string>

// using std::cout;
// using std::endl;
using namespace std;

int main() {

	cout << "Hello world!\n";			// ���ڸ����� ó���� ������ ���� ��
	printf("%s", "Hello woeld!\n");			// �Լ��� ���ͼ� ������ ���װ� ��

	cout << endl << endl << endl;

	cout << "�ʺ�: ";
	int width;
	cin >> width;
	cout << "�Է��� �ʺ�� " << width << "�Դϴ�. " << endl;

	cout << endl << endl << endl;

	char password[11];
	cout << "��ȣ: ";
	cin >> password;
	cout << "�Է��� ��ȣ�� \"" << password << "\"�Դϴ�. " << endl;
	cout << "(����� ©��)" << endl << endl;

	char passwordTwo[11];
	cout << "��ȣ: ";
	cin.getline(passwordTwo, 11, '\n');
	cout << "�Է��� ��ȣ�� \"" << passwordTwo << "\"�Դϴ�. " << endl;
	cout << "(����� �� ©��)" << endl;
	cout << "�տ� �ſ��� �Է��� ���Ⱑ �Է� ���ۿ��� �������� �ԷµǱ⵵" << endl;

	cout << endl << endl << endl;

	string song("�ȳ��ϼ���, �氡�氡");
	song = string("������ ����������");
	string singer("���� ��������");
	singer = "���� ��ǥ������ ������";

	cout << "�뷡�ҷ�! " << song << endl << "����? " << singer << endl;





	return 0;
}