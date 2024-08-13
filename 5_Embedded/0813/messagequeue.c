#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int shmid, pid;
    // 공유 메모리 영역을 할당할 포인터 변수
    char* shmaddr_parent, * shmaddr_child;
    // 키값 생성
    shmid = shmget((key_t)1234, 100, IPC_CREAT | 0664);
    if (shmid == -1) {
        perror("shmget error\n");
        exit(1);
    }

    // 자식 프로세스 생성
    pid = fork();
    if (pid > 0) { // 2.부모 프로세스
        // 자식 프로세스의 exit() 호출까지 대기
        wait(0);
        shmaddr_parent = (char*)shmat(shmid, (char*)NULL, 0);
        // 공유 메모리 값을 읽음(read)
        printf("%s\n", shmaddr_parent);
        shmdt((char*)shmaddr_parent);
    }
    else { // 1.자식 프로세스
        // 공유 메모리 키를 변수에 매핑
        shmaddr_child = (char*)shmat(shmid, (char*)NULL, 0);
        // 공유 메모리에 쓰기(write)
        strcpy((char*)shmaddr_child, "Hello Parent!");
        // 포인터 변수를 공유 메모리에서 해제
        shmdt((char*)shmaddr_child);
        sleep(10);
        exit(0);
    }
    // 공유메모리를 커널영역에서 삭제
    shmctl(shmid, IPC_RMID, (struct shmid_ds*)NULL);
    return 0;
}
