#ifndef JOSEPHU_H__
#define JOSEPHU_H__

typedef int datatype;

typedef struct node_st
{
	datatype data;
	struct node_st *next;
}list;

list * jose_create(int n);
void jose_show(list * const me);
void jose_kill(list **me,int n);


#endif
