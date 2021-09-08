#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"queue.h"

#include"gsp.h"

#define MAXSIZE (32)

typedef struct score_st
{
	int id;
	char name[MAXSIZE];
	int math;
}score;

void print_s(void *d)
{
	score *data = d;
	printf("%d\t%s\t%d\n", data->id, data->name, data->math);
}


int main()
{
	
	score tmp;
	score *data;
	struct list_head *cur;
	head_node *head = qu_create(sizeof(score));
	for(int i = 0; i < 6; i++)
	{
		tmp.id = i;
		snprintf(tmp.name, MAXSIZE, "stu%d", i);
		tmp.math = rand()%101;
		if(qu_enqueue(head, &tmp))
		{
			DBGLINE("enqueue failed!");
			exit(1);
		}

	}

	qu_travel(head, print_s);

	//test number
	printf("\nlist has %d member\n", qu_number(head));
	
	//test empty
	printf("\nlist is %sempty\n", qu_isempty(head)?"":"not ");

	if(qu_dequeue(head, &tmp)){
		DBGLINE("dequeue fail!");
		exit(1);
	}else{
		printf("dequeue data\n");
		print_s(&tmp);
		printf("rest data\n");
		qu_travel(head, print_s);
	}

	while(!qu_isempty(head)){
		if(qu_dequeue(head, &tmp)){
			DBGLINE("dequeue fail!");
			exit(1);
		}else{
			printf("dequeue data\n");
			print_s(&tmp);
		}
	}

	//test empty
	printf("\nlist is %sempty\n", qu_isempty(head)?"":"not ");

#if 0
	list_for_each(cur, &head)
	{
		data = list_entry(cur, score, node);
		
	}
#endif
	qu_clear(head);
}
