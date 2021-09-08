#ifndef LIST_H__
#define LIST_H__

typedef int datatype;

typedef struct node_st
{
	datatype data;
	struct node_st * next;
}list;

list *list_create();

/*插入指定的位置，不常用*/
int list_insert_at(list *,int i, datatype *);

/*按照顺序插入，常用*/
int list_insert_order(list *, datatype *);

int list_delete_at(list *,int i);

int list_delete(list *, datatype *);

int list_isempty();

void list_display(list *);

void list_destory(list *);
#endif
