#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<stdio.h>
#include"nohead.h"
#include"gsp.h"

/*insert in the first place*/
int list_insert(list **me, datatype *data)
{
	if(NULL == me || NULL == data)
		return PARAM_ERROR;
	list *new;
	new = malloc(sizeof(*new));
	if(NULL == new)
		return MALLOC_ERROR;

	new->data = *data;

	new->next = *me;
	*me = new;

	return 0;
}

void list_show(list *me)
{
	if(NULL == me)
		return;
	list * cur;
	printf("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
	printf("id\tname\tmath\tchinese\n");
	for(cur = me; cur != NULL; cur = cur->next)
	{
		printf("%d\t%s\t%d\t%d\n",cur->data.id,cur->data.name,cur->data.math,cur->data.chinese);
	}
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	return;
}

// delete the first node
int list_delete(list **me)
{
	if(NULL == me || NULL == *me)
		return PARAM_ERROR;

	list * tmp = *me;

	*me = (*me)->next;

	free(tmp);
	return 0;

}

// according to the id to show the students infomation
int list_find(list * const me, int id, list **res)
{
	list *tmp;
	*res = NULL;
	for(tmp = me;NULL != tmp;tmp = tmp->next)
	{
		if(tmp->data.id == id)
		{
			*res = tmp;
			//printf("%d\t%s\t%d\t%d\n",tmp->data.id,tmp->data.name,tmp->data.math,tmp->data.chinese);
			return 0;
		}
	}
	return -1;
}

void list_destroy(list * me)
{
	if(NULL == me)
		return;
	list *tmp;
	while(NULL != me)
	{
		tmp = me->next;
		free(me);
		me = tmp;
	}
	return;
}








