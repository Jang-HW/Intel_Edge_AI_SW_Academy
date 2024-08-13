#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void* read(void* arg);
void* accu(void* arg);
// 참 거짓으로 제어하는 변수
static sem_t sem_one;
static sem_t sem_two;
static int num;

int main(int argc, char* argv[])
{
	pthread_t id_t1, id_t2;
	// 각가의 초기 값
		// sem_two가 1인 이유는 처음에 막아두고 시작하기 위해
	sem_init(&sem_one, 0, 0);
	sem_init(&sem_two, 0, 1);

	// 스레드 생성
	pthread_create(&id_t1, NULL, read, NULL);
	pthread_create(&id_t2, NULL, accu, NULL);

	// 스레드 종료 대기
	pthread_join(id_t1, NULL);
	pthread_join(id_t2, NULL);

	// 세마포어 등록 해제
	sem_destroy(&sem_one);
	sem_destroy(&sem_two);
	return 0;
}

void* read(void* arg)
{
	int i;
	for (i = 0; i < 5; i++)
	{
		fputs("Input num: ", stdout);

		// 사용 가능 여부 확인 (할당)
		sem_wait(&sem_two);
		// 값 작성 (변경)
		scanf("%d", &num);
		// 다 썻다고 반대문 열고 나감 (반환)
		sem_post(&sem_one);
	}
	return NULL;
}
void* accu(void* arg)
{
	int sum = 0, i;
	for (i = 0; i < 5; i++)
	{
		sem_wait(&sem_one);
		sum += num;
		sem_post(&sem_two);
	}
	printf("Result: %d \n", sum);
	return NULL;
}