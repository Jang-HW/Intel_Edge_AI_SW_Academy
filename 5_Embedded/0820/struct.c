#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct student {
	int num;
	char * name;
//	char  name[10];
};

struct student test = {
	.num = 1,
	.name = "test"
};
int main()
{
	char name[30];
	printf("num : %d, name : %s\n",test.num, test.name);

	struct student st = {1, "ksh"};
	st.num = 2;
	st.name = "bbb";
//	strcpy(st.name,"bbb");
	printf("num : %d, name : %s\n",st.num, st.name);

	printf("학번을 입력하세요! : ");
	scanf("%d",&st.num);
	printf("이름을 입력하세요! : ");
	scanf("%s",name);

//	st.name = (char *)malloc(strlen(name)+1);
//	memcpy(st.name,0x00,strlen(name)+1)
	st.name = (char *)calloc(sizeof(char) ,strlen(name)+1);
	strcpy(st.name,name);

	printf("num : %d, name : %s\n",st.num, st.name);


	struct student sa[2] = {{1, "ksh"},{2,"aaa"}};
	printf("num : %d, name : %s\n",sa[0].num, sa[0].name);
	printf("num : %d, name : %s\n",sa[1].num, sa[1].name);

	struct student * pSt;
	pSt = &st;
	printf("num : %d, name : %s\n",(*pSt).num, (*pSt).name);
	printf("num : %d, name : %s\n",pSt->num, pSt->name);
}
