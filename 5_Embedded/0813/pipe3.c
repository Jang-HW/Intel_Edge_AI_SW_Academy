#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char* argv[])
{
	// file descriptor
	int fds1[2], fds2[2];
	char str1[] = "Who are you?";
	char str2[] = "Thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds1), pipe(fds2);
	pid = fork();

	if (pid == 0)
	{
		// child
		// 자기가 쓴거를 실수로 자기가 읽는 거를 방지하기 위한 코드를 추기
		close(fds1[0]);
		write(fds1[1], str1, sizeof(str1));
		read(fds2[0], buf, BUF_SIZE);
		printf("Child proc output: %s \n", buf);
	}
	else
	{
		// parents 
		// 자기가 쓴거를 실수로 자기가 읽는 거를 방지하기 위한 코드를 추기
		close(fds2[0]);
		read(fds1[0], buf, BUF_SIZE);
		printf("Parent proc output: %s \n", buf);
		write(fds2[1], str2, sizeof(str2));
		sleep(3);
	}
	return 0;
}