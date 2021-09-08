#ifndef SQSTACK_H__
#define SQSTACK_H__

#define STACKMAXSIZE (64)

typedef int stackdatatype;

typedef struct stack_node_st
{
	stackdatatype data[STACKMAXSIZE];
	int top;
}sqstack;


sqstack * st_create(void);

int st_isempty(sqstack *);

int st_isfull(sqstack *);

int st_number(sqstack *);

int st_push(sqstack *, stackdatatype *);

int st_pop(sqstack *, stackdatatype *);

int st_top(sqstack *, stackdatatype *); // just see the top data, not pop

void st_travel(sqstack *);

void st_destroy(sqstack *);



#endif