#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"queue.h"
#include"gsp.h"

int qu_isempty(queue *);

int qu_number(queue *);

int qu_enqueue(queue *, datatype *);

int qu_dequeue(queue *, datatype *);

void qu_travel(queue *);

void qu_clear(queue *);

void qu_destroy(queue *);


int main()
{
	queue *sq;
	datatype tmp;
	int i;
	sq = qu_create();

	for(i=0; i<(MAXSIZE-21); i++)
	{
		tmp = i;
		//tmp = rand()%100;
		if(qu_enqueue(sq, &tmp))
			exit(1);
	}
	qu_travel(sq);

	printf("\nqueue is %sempty!\n", qu_isempty(sq)?"":"not ");
	printf("queue is %sfull!\n", qu_isfull(sq)?"":"not ");
	printf("queue num is %d\n\n", qu_number(sq));

	while(!qu_isempty(sq))
	{
		if(qu_dequeue(sq, &tmp))
			exit(1);
		printf("dequeue data %d\n", tmp);
	}

	qu_destroy(sq);
	
	exit(0);
}
