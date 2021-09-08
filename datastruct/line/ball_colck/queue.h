#ifndef QUEUE_H__
#define QUEUE_H__

#define MAXSIZE (64)

typedef int queuedatatype;

typedef struct node_st
{
	int head,tail;
	queuedatatype data[MAXSIZE];
}queue;

queue *qu_create();

int qu_isempty(queue *);

int qu_isfull(queue *);

int qu_number(queue *);

int qu_enqueue(queue *, queuedatatype *);

int qu_dequeue(queue *, queuedatatype *);

void qu_travel(queue *);

void qu_clear(queue *);

void qu_destroy(queue *);

#endif