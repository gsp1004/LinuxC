#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<stdio.h>
#include"josephu.h"
#include"gsp.h"
//单向环链
list * jose_create(int n)
{
	if(n<=0)
		return NULL;

	list * ret;
	ret = malloc(sizeof(*ret));
	if(ret == NULL)
		return NULL;

	ret->data = 1;
	ret->next = ret;

	list * cur = ret;
	for(int i=2;i<=n;i++)
	{
		list * new_node;
		new_node = malloc(sizeof(*new_node));
		if(new_node == NULL)
			return NULL;

		new_node->data = i;
		new_node->next = cur->next;
		cur->next = new_node;
		cur = cur->next;
	}

	return ret;
}
void jose_show(list * const me)
{
	if(NULL == me)
		return;
	
	list * cur = me;
	printf("\nshow list vvvvvvvvvvvvvv\n");
	do{
		printf("%d\t",cur->data);
		cur = cur->next;
	}
	while(NULL != cur && cur != me);
	printf("\nshow finished ^^^^^^^^^^\n");
	return;
}

void jose_kill(list **me,int n)
{
	if(n<=0)
		return;
	
	list * lp, *cur, *rp;
	cur = *me;
	printf("\ndelete started!\n");
	if(n == 1)
	{
		while(cur->next != *me){
			rp = cur->next;
			printf("%d\t",cur->data);
			free(cur);
			cur = rp;
		}

		cur->next = cur;
		*me = cur;
	}
	else
	{
		while(cur->next != cur){
			for(int i=1;i<n;i++)
			{
				lp = cur;  // 记录前驱节点
				cur = cur->next;  // 记录待删除节点
			}
			lp->next = cur->next;
			printf("%d\t",cur->data);
			free(cur);
			cur = lp->next;
		}
		*me = cur;
	}
	printf("\ndelete finished!\n");
	return;
}

