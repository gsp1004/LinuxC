#include<stdio.h>
#include<stdlib.h>
#include "list.h"

#define NAMESIZE (32)

struct socre_st
{
	int id;
	char name[NAMESIZE];
	int math;
	int chinese;
	struct list_head node;
};

void print_s(void *s)
{
	struct socre_st * data = (struct socre_st *)s;
	printf("%d\t%s\t%d\t%d\n",data->id, data->name, data->math, data->chinese);
}

int main()
{
	struct socre_st *datap;
	struct list_head *cur;
	LIST_HEAD(head);

	for(int i=0; i<7; i++){
		datap = malloc(sizeof(*datap));
		if(NULL == datap)
			exit(1);

		datap->id = i;
		snprintf(datap->name, NAMESIZE, "stu%d",i);
		datap->math = rand()%101;
		datap->chinese = rand()%101;

		//list_add(&datap->node, &head);
		list_add_tail(&datap->node, &head);
	}

	list_for_each(cur, &head)
	{
		datap = list_entry(cur, struct socre_st, node);
		print_s(datap);
	}

	list_for_each(cur, &head)
	{
		datap = list_entry(cur, struct socre_st, node);
		if(5 == datap->id){
			printf("found id = %d infomation\n",datap->id);
			print_s(datap);
			break;
		}
	}

	if(cur == &head)
		printf("Not found!\n");
	
}
