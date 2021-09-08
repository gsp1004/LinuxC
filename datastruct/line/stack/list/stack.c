#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"stack.h"

STACK *stack_create(int initsize)
{
	return llist_create(initsize);
}

int stack_push(STACK *st, const void *data)
{
	return llist_insert(st,data,LLIST_FORWARD);
}


int always_return0(const void *p1, const void *p2)
{
	return 0;
}

int stack_pop(STACK *st, void *data)
{
	return llist_fetch(st, NULL, always_return0, data);
}

void stack_destroy(STACK *st)
{
	llist_destroy(st);
	return;
}

