#ifndef NOHEAD_H__
#define NOHEAD_H__

#define NAMESIZE 32



typedef struct node_st
{
	int coef;
	int exp;
	struct node_st *next;
}list;

//typedef  datatype;

list * poly_create(int a[][2],int n);
void poly_show(list *me);
int poly_union(list * const p1, list * const p2);


#endif
