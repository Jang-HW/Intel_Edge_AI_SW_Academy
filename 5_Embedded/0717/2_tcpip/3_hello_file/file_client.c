#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc, char* argv[])
{
	int n, in;
	int sock, str_len;
	char buf[100];
	struct sockaddr_in serv_addr;

	if (argc != 4) {
		printf("Usage : %s <IP> <port> <file>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
		error_handling("connect() error");

	/* 복사의 원본이 되는 파일을 읽기 모드로 연다 */
	if ((in = open(argv[3], O_RDONLY)) < 0){
		perror(argv[3]);
		return 1;
	}
	
	do {
		n = read(in, buf, sizeof(buf));
		
		if(n>0){
	        write(sock, buf, n);
		}
		else if(n == 0)
			break;
		
		else 
			perror("read");

	} while(1);

	printf("Done..\n");
	close(sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

