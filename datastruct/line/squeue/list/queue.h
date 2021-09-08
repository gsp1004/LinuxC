#ifndef QUEUE_H__
#define QUEUE_H__
#include "list.h"

typedef void head_node;
/**
 * qu_create - create a queue
 * @1: data size
*/
head_node * qu_create(int);

/**
 * qu_enqueue
 * @1: the queue
 * @2: pointer of the data
*/
int qu_enqueue(head_node *, void *data);

/**
 * qu_isempty
 * @1: the queue
 * return: 1 -- is empty
 *         0 -- not empty
*/
int qu_isempty(head_node *);

/**
 * qu_number
 * @1: the queue
 * return: >0, the number of member in queue
*/
int qu_number(head_node *);

/**
 * qu_enqueue
 * @1: the queue
 * @2: NULL if you do not need the data
*/
int qu_dequeue(head_node *, void *data);

/**
 * qu_travel
 * @1: the queue
 * @2: a callback function to tell me how to show the data
*/
void qu_travel(head_node *, void (*)(void*));

/**
 * qu_clear -- destroy the queue
 * @1: the queue
*/
void qu_clear(head_node *);

#endif
