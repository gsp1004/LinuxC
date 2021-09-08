#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"stack.h"
#include"gsp.h"

typedef struct score_st
{
	int id;
	int math;
}score;

int main()
{
	STACK * st;
	score tmp;
	int i,ret;
	st = stack_create(sizeof(score));
	if(NULL == st)
		exit(1);

	for(i = 0; i< 7; i++)
	{
		tmp.id = i;
		tmp.math = rand()%101;
		if(ret = stack_push(st,&tmp)){
			printf("ret = %d\n",ret);
			exit(1);
		}

	}

	while(1){
		if(stack_pop(st,&tmp))
			break;
		printf("%d\t%d\n",tmp.id, tmp.math);
	}

	stack_destroy(st);
}
