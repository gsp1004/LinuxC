#ifndef STACK_H__
#define STACK_H__

#include "llist.h"

typedef LLIST STACK;

STACK *stack_create(int initsize);

int stack_push(STACK *st, const void *data);

int stack_pop(STACK *st, void *data);

void stack_destroy(STACK *st);

#endif
