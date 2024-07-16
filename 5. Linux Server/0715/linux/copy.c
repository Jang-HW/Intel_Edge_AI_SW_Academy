#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main (int argv, char *argc[])
{
	int n, in, out;
	char buf[1024];
	/* 명령행 인수로 복사할 파일 명이 없는 경우에 에러를 출력하고 종료한다 */
	if (argv < 3){
		write(2, "Usage : copy file1 file2\n", 25);
		return -1;
	}

	/* 복사의 원본이 되는 파일을 읽기 모드로 연다 */
	if ((in = open(argc[1], O_RDONLY)) < 0){
		perror(argc[1]);
		return -1;
	}

	/* 복사할 결과 파일을 쓰기 모드 (새로운 파일 생성 | 기존에 파일 내용 지움)로 연다 */
	if((out = open(argc[2], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) < 0){
		perror(argc[2]);
		return -1;
	}

	/* 원본 파일의 내용을 읽어서 복사할 파일에 쓴다 */
	while ((n = read(in, buf, sizeof(buf))) > 0)
		write(out, buf, n);

	/* 열린 파일들을 닫는다. */
	close(out);
	close(in);

	/* 프로그램의 정상 종료 시 0을 반환한다 */
	return 0;
}

