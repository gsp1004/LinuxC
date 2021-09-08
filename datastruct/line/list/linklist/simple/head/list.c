#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<stdio.h>
#include"list.h"

list *list_create()
{
	list *me;
	me = malloc(sizeof(list));
	if(NULL == me)
	{
		fprintf(stderr, "malloc error!\n");
		return NULL;
	}

	// init
	me->next = NULL;

	return me;
}

/*
   插入指定的位置，不常用
   第一个有效节点定义为第0个节点
retrun: -1 -- param error
        -2 -- wrong position
        -3 -- malloc failed
		0  -- success
 */
int list_insert_at(list *me,int i, datatype *data)
{
	// param judge
	if(i < 0 || NULL == me || NULL == data)
	{
		fprintf(stderr, "param invalid\n");
		return -1;
	}

	list *insert_node;
	int j = 0;

	for(j=0; j<i; j++) // if i=0, do not loop
	{
		me = me->next;
		if(NULL == me)
		{
			fprintf(stderr, "error position %d,only %d member in the list!\n", i, j);
			return -2;
		}
	}

	insert_node = malloc(sizeof(*insert_node));
	if(NULL == insert_node)
	{
		fprintf(stderr, "malloc failed!\n");
		return -3;
	}

	insert_node->data = *data;
	insert_node->next = me->next;
	me->next = insert_node;
	return 0;
}

/*按照顺序插入，常用，比如这个从小到大排序*/
int list_insert_order(list *me, datatype *data)
{
	if(NULL == me || NULL == data)
		return -1;


	list *llink, *rlink;
	for(llink = me,rlink = me->next;
			;
			llink = rlink, rlink = rlink->next)
	{
		if(rlink == NULL || rlink->data >= *data)
		{
			// insert here
			list *insert_node = malloc(sizeof(list));
			if(NULL == insert_node)
			{
				fprintf(stderr, "malloc failed!\n");
				return -3;
			}

			llink->next = insert_node;
			insert_node->next = rlink;
			insert_node->data = *data;
			return 0;
		}
	}

}

/*
 删除指定位置的元素，第一个有效元素的序号是0
 */
int list_delete_at(list *me,int i)
{
	// param judge
	if(i < 0 || NULL == me)
	{
		fprintf(stderr, "param invalid\n");
		return -1;
	}

	int j;
	for(j=0; j<i; j++) // if i=0, do not loop
	{
		me = me->next;
		if(NULL == me)
		{
			fprintf(stderr, "error position %d,only %d member in the list!\n", i, j);
			return -2;
		}
	}
	// 当前找到了前驱
	if(me->next == NULL)
	{
		return -3;
	}

	list * delete_node = me->next;
	me->next = delete_node->next;
	free(delete_node);
	
	return 0;
}

/*
retrun: -1 -- no such data
         0 -- success
 */
int list_delete(list *me, datatype *data)
{
	list *llink,*curlink;
	for(llink = me, curlink = me->next; curlink != NULL; llink = curlink, curlink = curlink->next)
	{
		if(curlink->data == *data)
		{
			llink->next = curlink->next;
			free(curlink);
			return 0;
		}
	}
	return -1; // node == NULL
}

int list_isempty(list *me)
{
	if(NULL == me || me->next == NULL)
		return 1;
	else
		return 0;
}

void list_display(list *me)
{
	if(NULL == me)
	{
		printf("bad NULL param\n");
		return;
	}

	list *node = me;
	if(list_isempty(node))
	{
		printf("display nothing because of empty list\n");
		return;
	}

	do
	{
		node = node->next;
		printf("%d\t",node->data);
	}
	while(node->next != NULL);
	printf("\n");

	return;
}

void list_destory(list *me)
{
	if(NULL == me)
		return;

	list *node;
#if 1 // two ways to free, both OK
	list *next;
	for(node = me; node !=NULL; node=next)
	{
		next = node->next;
		free(node);
	}
#else
	do
	{
		node = me->next;
		free(me);
		me = node;
	}
	while(me != NULL);
#endif
	return;
}
