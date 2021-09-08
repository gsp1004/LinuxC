#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"sqlist.h"

/*
use malloc to get memery
return NULL for fail
*/
sqlist *sqlist_create()
{
	sqlist *me;
	me = malloc(sizeof(*me));
	if(NULL == me)
		return NULL;
	// INIT
	me->last = -1;
	return me;
}

void sqlist_create1(sqlist **ptr)  // fail to return NULL
{
	*ptr = malloc(sizeof(** ptr));
	if(NULL == *ptr)
		return;

	(*ptr)->last = -1;
	return;
}

int sqlist_insert(sqlist *me, int i, datatype *data)  // use pointer is better,  return 0 for success
{
	if(me->last == DATASIZE-1) // arr full
		return -1;

	if(i > me->last + 1 || i < 0) // param err
		return -2;

	int j;

	for(j = me->last; j >= i; j--)
	{
		//me->data[j+1] = me->data[j];
		// use memcpy for different data type
		memcpy(&me->data[j+1], &me->data[j], sizeof(me->data[j]));
	}
	
	memcpy(&me->data[i], data, sizeof(me->data[j+1]));
	me->last++;
	return 0;
}

int sqlist_delete(sqlist *me, int i)
{
	if(i < 0 || i > me->last)
		return -1;

	int j;
	for(j = i+1; j <= me->last; j++)
	{
		me->data[j-1] = me->data[j];
	}
	me->last--;

	return 0;
}

int sqlist_find(sqlist *me, datatype *data)
{
	int j;

	if(sqlist_isempty(me) == 1)
		return -1;

	for(j=0; j<=me->last; j++)
	{
		if(me->data[j] == *data)
			return j;
	}

	return -2;  //not found
}

int sqlist_isempty(sqlist *me)
{
	return (me->last == -1);
}

int sqlist_setempty(sqlist *me)
{
	me->last = -1;
	return 0;
}

int sqlist_getnum(sqlist *me)
{
	return me->last+1;
}

void sqlist_display(sqlist *me)
{
	int i;
	if(sqlist_isempty(me) == 1)
		return;
	for(i=0; i<=me->last; i++)
		printf("%d\t",me->data[i]);

	printf("\n");
	return;
}

/*
 * func: put member of list2 to list1
 * return: -1 -- bad param
 *         -2 -- 
 *         -2 -- 
 *         -2 -- 
 *         -2 -- 
 *         -2 -- 
 */
int sqlist_union(sqlist *list1, sqlist *list2)
{
	if(NULL == list1 || NULL == list2)
		return -1;

	int i,err,num;
	for(i=0; i<= list2->last;i++)
	{
		if(sqlist_find(list1,&list2->data[i]) >=0) // found, do nothing
			continue;
		else
		{
			num = sqlist_getnum(list1);
			//err = sqlist_insert(list1, sqlist_getnum(list1), &list2->data[i]);
			err = sqlist_insert(list1, num, &list2->data[i]);
			printf("err = %d, num = %d\n", err, num);
		}
	}
	return 0;
}

int sqlist_destroy(sqlist *me)  // return 0 for success
{
	if(NULL != me)
	{
		free(me);
		return 0;
	}
	else
	{
		fprintf(stderr, "bad pointer NULL!\n");
		return -1;
	}
}	
