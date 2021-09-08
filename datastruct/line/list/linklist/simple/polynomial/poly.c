#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<stdio.h>
#include"poly.h"
#include"gsp.h"

/*
带头结点的链表
*/
list * poly_create(int a[][2],int n)
{
	list * me;
	list *newnode;
	list *cur;

	me = malloc(sizeof(*me));
	if(NULL == me)
		return NULL;

	me->next = NULL;

	cur = me;
	for(int i=0;i<n;i++)
	{
		newnode = malloc(sizeof(*newnode));
		if(NULL == newnode)
			return NULL;

		newnode->coef = a[i][0];
		newnode->exp = a[i][1];
		newnode->next = NULL;

		cur->next = newnode;
		cur = newnode;
	}
	return me;
}

void poly_show(list *me)
{
	list *cur;

	for(cur = me->next;cur != NULL; cur = cur->next)
	{
		printf("%dx^%d ",cur->coef,cur->exp);
	}
	printf("\n");
	return ;
}

int poly_union(list * const p1, list * const p2)
{
	list *p,*q, *lp;
	lp = p1;
	p = p1->next;
	q = p2->next;

	while(NULL != p && NULL != q)
	{
		if(p->exp < q->exp)
		{
			lp->next = p;
			lp = p;
			p = p->next;
		}
		else if(p->exp > q->exp)
		{
			lp->next = q;
			lp = q;
			q = q->next;
		}
		else //if(p->exp == q->exp)
		{
			p->coef += q->coef;
			#if 1
			if(p->coef)
			{
				lp->next = p;
				lp = p;
			}
			p = p->next;
			q = q->next;
			
			#else // 考虑要free节点的代码
			if(p->coef)
			{
				lp->next = p;
				lp = p;
				p = p->next;
				q = q->next;
			}
			else
			{
				list *tmp = p;
				p = p->next;
				q = q->next;
				free(tmp);
			}
			#endif
		}
	}
	
	if(NULL == q)
	{
		lp->next = p;
		return 0;
	}
	if(NULL == p)  // now q != NULL, 
	{
		lp->next = q;
		return 0;
	}

	return -1;  // should not happen!
}

