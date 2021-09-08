#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<time.h>
#include"llist.h"
#include"gsp.h"
/*
双向链表
*/

#define NAMESIZE 32

typedef struct score_st
{
	int id;
	char name[NAMESIZE];
	int math;
	int chinese;
}score;

/*
功能：传入要打印的数据指针，打印出里面的内容
*/
static void callback_print(const void * s)
{
	score *data = (score *)s;
	printf("%d\t%s\t%d\t%d\n",data->id, data->name, data->math, data->chinese);
	return;
}

static int id_cmp(const void *key, const void *data)
{
	const int * k = key;
	const score * r = data;

	return !!(*k - r->id);
}

static int name_cmp(const void *key, const void *data)
{
	const char * k = key;
	const score * r = data;

	return strcmp(k, r->name);
}


int main(int argc, char *argv[])
{
	LLIST *handler;
	int initsize = sizeof(struct score_st);
	score data;
	handler = llist_create(initsize);
	if(NULL == handler)
		exit(1);
	for(int i=0;i<7;i++){
		data.id = i;
		snprintf(data.name, NAMESIZE, "stu%d", i);
		data.math = rand()%101;
		data.chinese = rand()%101;
		//llist_insert(handler, &data, LLIST_FORWARD);
		llist_insert(handler, &data, LLIST_BACKWARD);
	}

#if 1
	printf("********** show list ***********\n");
	printf("id\tname\tmath\tchinese\n");
	llist_travel(handler, callback_print);
	printf("********************************\n");
#else // demo to use llist_travel2
	if(0 != llist_travel2(handler, &data)){
		exit(1);
	}
	else{
		printf("id\tmath\tchinese\n");
		do{
			printf("%d\t%d\t%d\n",data.id, data.math, data.chinese);
		}
		while(0 == llist_travel2(NULL, &data));
	}
#endif

	// test find function
	printf("test find function\n");
	int id = 3;
	score * ret_data;
	ret_data = llist_find(handler, &id, id_cmp);
	if(NULL == ret_data)
		DBGLINE("Can not find!\n");
	else
		callback_print(ret_data);
	printf("--------------------------------\n");

	//test delete
	printf("test delete function by id\n");
	id = 5;
	if(llist_delete(handler, &id, id_cmp))
	{
		DBGLINE("Can not delete\n");
	}
	else
	{
		llist_travel(handler, callback_print);
	}
	printf("--------------------------------\n");

	printf("test delete function by name\n");
	char * delete_name = "stu0";
	if(llist_delete(handler, delete_name, name_cmp))
	{
		DBGLINE("Can not delete\n");
	}
	else
	{
		llist_travel(handler, callback_print);
	}
	printf("--------------------------------\n");

	//test fetch
	printf("test fetch function\n");
	id = 4;
	if(llist_fetch(handler, &id, id_cmp, &data))
	{
		DBGLINE("Can not fetch\n");
	}
	else
	{
		printf("fetch data\n");
		callback_print(&data);
		printf("rest data\n");
		llist_travel(handler, callback_print);
	}
	printf("--------------------------------\n");

	llist_destroy(handler);
	return 0;
}
