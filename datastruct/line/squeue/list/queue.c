#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"queue.h"

#if 0
typedef struct head_node_st
{
	int size;  // data size
	list_head head;
}head_node;
#endif

struct head_node_st
{
	int datasize;  // data size
	struct list_head head;
};

typedef struct data_node_st
{
	struct list_head node;
	char data[0];
}data_node;

void *qu_create(int initsize)
{
	struct head_node_st *header;
	header = malloc(sizeof(*header));
	if(NULL == header)
		return NULL;
	header->datasize = initsize;
	header->head.next = &header->head;
	header->head.prev = &header->head;
	return header;
}

int qu_enqueue(head_node *header, void *data)
{
	struct head_node_st *h = header;
	struct list_head *head = &(((struct head_node_st *)header)->head);
	data_node *d = malloc(sizeof(*d) + h->datasize);
	if(NULL == d)
		return -1;
	memcpy(d->data, data, h->datasize);
	list_add_tail(&(d->node), head);
	return 0;
}

int qu_isempty(head_node *header)
{
	struct head_node_st * h = header;
	struct list_head *head = &(h->head);
	return (head->next == head);
}

int qu_number(head_node *header)
{
	int i = 0;
	struct head_node_st * h = header;
	struct list_head *head = &(h->head);
	struct list_head *tmp = head;
	for(i=0; tmp->next != head; ++i,tmp = tmp->next);

	return i;
}

int qu_dequeue(head_node *header, void *data)
{
	struct head_node_st * h = header;
	struct list_head *head = &(h->head);
	struct list_head *dequeue_node;
	if(qu_isempty(head))
		return -1;

	dequeue_node = head->next;
	list_del(dequeue_node);
	
	if(data){
		memcpy(data, list_entry(dequeue_node, data_node, node)->data, h->datasize);
	}
	return 0;
}

void qu_travel(head_node *header, void (*func)(void*))
{
	struct head_node_st * h = header;
	struct list_head *head = &(h->head);
	struct list_head *cur;
	data_node *data;
	printf("travel start----------------\n");
	list_for_each(cur, head)
	{
		data = list_entry(cur, data_node, node);
		func(data->data);
	}
	printf("travel end------------------\n");
	return;
}

void qu_clear(head_node *header)
{
	struct head_node_st * h = header;
	struct list_head *head = &(h->head);
	struct list_head *cur, *tmp;
	data_node *data;
	list_for_each_safe(cur, tmp, head)
	{
		data = list_entry(cur, data_node, node);
		free(data);
	}
	free(header);
}

