#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// 메세지 큐 전송 프로그램과 동일한 메세지 타입을 생성 
typedef struct msgbuf {
    long type;
    char text[50];
} MsgBuf;

int main(void) {
    MsgBuf msg;
    int msgid, len;
    // 메세지 큐 전송 프로그램과 동일한 메세지 id 사용 
    key_t key = 1234;
    // 메세지 키도 동일하게 생성
    if ((msgid = msgget(key, IPC_CREAT | 0644)) < 0) {
        perror("msgget");
        exit(1);
    }
    // 위에서 입력한 키값을 가진 메세지 큐를 수신 
    len = msgrcv(msgid, &msg, 50, 0, 0);
    printf("Received Message is [%d] %s\n", len, msg.text);
    // IPC_RMID : 읽고 나서 메세지 삭제
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}