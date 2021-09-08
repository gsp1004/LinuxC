#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"sqstack.h"


sqstack * st_create(void)
{
	sqstack *st;
	st = malloc(sizeof(*st));
	if(NULL == st)
		return NULL;

	st->top = -1;
	return st;
}


// 1 -- empty
int st_isempty(sqstack * st)
{
	return (st->top == -1);
}

// 1 -- full
int st_isfull(sqstack * st)
{
	return (st->top == STACKMAXSIZE-1);
}

int st_number(sqstack * st)
{
	return (st->top + 1);
}

int st_push(sqstack * st, stackdatatype *data)
{
	if(st_isfull(st))
		return -1;

	st->data[++st->top] = *data;
	return 0;
}



int st_pop(sqstack * st, stackdatatype *data)
{
	if(st_isempty(st))
		return -1;
	memcpy(data, &st->data[st->top -- ], sizeof(stackdatatype));
	return 0;
}



int st_top(sqstack * st, stackdatatype *data)
{
	if(st_isempty(st))
		return -1;
	memcpy(data, &st->data[st->top], sizeof(stackdatatype));
	return 0;

}

 // just see the top data, not pop

void st_travel(sqstack * st)
{
	if(st_isempty(st))
		return;
	for(int i=0; i<= st->top; i ++)
	{
		printf("%d\t",st->data[i]);
	}
	printf("\n");
}



void st_destroy(sqstack * st)
{
	if(NULL != st)
		free(st);
}


