#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include"queue.h"


queue *qu_create()
{
	queue *sq;

	sq = malloc(sizeof(*sq));
	if(NULL == sq)
		return NULL;
	sq->head = 0;
	sq->tail = 0;
	return sq;
}


int qu_isempty(queue *sq)
{
	return (sq->head == sq->tail);
}

int qu_isfull(queue *sq)
{
	return ((sq->tail + 1)%MAXSIZE == sq->head);
}

int qu_number(queue *sq)
{
	return ((sq->tail - sq->head + MAXSIZE)%MAXSIZE);
}

int qu_enqueue(queue *sq, queuedatatype *data)
{
	if(qu_isfull(sq))
		return -1;

	sq->tail = (sq->tail + 1) % MAXSIZE;
	//sq->data[sq->tail] = *data;
	memcpy(&(sq->data[sq->tail]), data, sizeof(queuedatatype));
	return 0;
}


int qu_dequeue(queue *sq, queuedatatype *data)
{
	if(qu_isempty(sq))
		return -1;

	sq->head = (sq->head + 1) % MAXSIZE;
	//  *data = sq->data[sq->head];
	memcpy(data, &(sq->data[sq->head]), sizeof(queuedatatype));
	return 0;
}


void qu_travel(queue *sq)
{
	int i;
	printf("travel start\nhead\t");
	for(i = sq->head+1; i != sq->tail; i = (i+1)%MAXSIZE)
	{
		printf("%d ", sq->data[i]);
	}
	printf("%d\t", sq->data[i]);  // tail data
	printf("tail\ntravel finish\n");
	return;
}


void qu_clear(queue *sq)
{
	sq->head = 0;
	sq->tail = 0;
	return;
}


void qu_destroy(queue *sq)
{
	free(sq);
}


