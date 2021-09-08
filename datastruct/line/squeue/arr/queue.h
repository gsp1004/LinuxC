#ifndef QUEUE_H__
#define QUEUE_H__

#define MAXSIZE (32)

typedef int datatype;

typedef struct node_st
{
	int head,tail;
	datatype data[MAXSIZE];
}queue;

queue *qu_create();

int qu_isempty(queue *);

int qu_isfull(queue *);

int qu_number(queue *);

int qu_enqueue(queue *, datatype *);

int qu_dequeue(queue *, datatype *);

void qu_travel(queue *);

void qu_clear(queue *);

void qu_destroy(queue *);

#endif