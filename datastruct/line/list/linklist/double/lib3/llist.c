#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"llist.h"
#include"gsp.h"

int llist_insert(LLIST *me, const void *data, insert_mode mode);
void * llist_find(LLIST *me, const void *key, llist_cmp *cmp);
int llist_delete(LLIST *me, const void *key, llist_cmp *cmp);
int llist_fetch(LLIST *me, const void *key, llist_cmp *cmp, void *data);
void llist_travel(LLIST *me, llist_op *func);
int llist_travel2(LLIST *me, void *data);


LLIST *llist_create(int initsize)
{
	LLIST *ret;
	#ifdef USE_POINTER_DATA
	ret = malloc(sizeof(*ret));
	#else
	ret = malloc(sizeof(*ret) + initsize);
	#endif
	if(NULL == ret)
		return NULL;
	ret->size = initsize;
	#ifdef USE_POINTER_DATA
	ret->head.data = NULL;
	#endif
	ret->head.next = & ret->head;
	ret->head.prev = & ret->head;

	ret->insert = llist_insert;
	ret->find = llist_find;
	ret->delete = llist_delete;
	ret->fetch = llist_fetch;
	ret->travel = llist_travel;
	ret->travel2 = llist_travel2;
	return ret;
}

int llist_insert(LLIST *me, const void *data, insert_mode mode)
{
	if(NULL == me || NULL == data)
		return -1;

	node *insert_node = NULL;
	insert_node = malloc(sizeof(*insert_node) + me->size);
	if(insert_node == NULL)
		return -1;

	#ifdef USE_POINTER_DATA
	insert_node->data = malloc(me->size);
	if(insert_node->data == NULL)
		return -2;
	#endif

	memcpy(insert_node->data, data, me->size);
	if(mode == LLIST_FORWARD)
	{
		insert_node->next = me->head.next;
		insert_node->prev = &(me->head);
		
		insert_node->next->prev = insert_node;
		insert_node->prev->next = insert_node;
	}
	else if(mode == LLIST_BACKWARD)
	{
		insert_node->next = &(me->head);
		insert_node->prev = me->head.prev;
		
		insert_node->next->prev = insert_node;
		insert_node->prev->next = insert_node;
	}
	else
	{
		return -3; // unkonwn mode
	}
	return 0;
}

/*
param 1: list header
param 2: the key word to find
param 3: the callback function offered by user
return : the node we found, if not found ,return NULL
*/
static node *find_(LLIST *me, const void *key, llist_cmp *cmp)
{
	node *cur;
	for(cur = me->head.next; cur != &me->head; cur = cur->next)
	{
		if(0 == cmp(key, cur->data))
			return cur;
	}
	return NULL;
}

/*
find the data that user need and return
if not found ,return NULL
*/
void * llist_find(LLIST *me, const void *key, llist_cmp *cmp)
{
	node * find_node;
	find_node = find_(me,key,cmp);
	if(find_node == NULL)
		return NULL;
	else
		return find_node->data;
}

/*????????????????????????
return: -1-- not found
		0 -- success
*/
int llist_delete(LLIST *me, const void *key, llist_cmp *cmp)
{
	node * find_node;
	find_node = find_(me,key,cmp);
	if(find_node == NULL)
		return -1;

	// ??????
	find_node->prev->next = find_node->next;
	find_node->next->prev = find_node->prev;

	// ????????????
	#ifdef USE_POINTER_DATA
	free(find_node->data);
	#endif
	free(find_node);
	return 0;
}

/*????????????????????????????????????*/
int llist_fetch(LLIST *me, const void *key, llist_cmp *cmp, void *data)
{
	node * find_node;
	find_node = find_(me,key,cmp);
	if(find_node == NULL)
		return -1;

	// ??????
	find_node->prev->next = find_node->next;
	find_node->next->prev = find_node->prev;

	// ????????????
	if(NULL != data)
		memcpy(data, find_node->data, me->size);

	#ifdef USE_POINTER_DATA
	free(find_node->data);
	#endif
	free(find_node);
	return 0;
}

/*
?????????????????????????????????????????????????????????
??????????????????????????????????????????
*/
void llist_travel(LLIST *me, llist_op *func)
{
	node * cur;
	for(cur = me->head.next; cur != &me->head; cur = cur->next)
	{
		func(cur->data);
	}
	return;
}


/*
???????????????????????????????????????data
????????????????????????????????????????????????1?????????????????????????????????????????????1??????NULL
????????????1?????????????????????????????????NULL??????????????????????????????
????????????2?????????????????????????????????
????????????-1 -- data?????????NULL
        -2 -- ??????????????????????????????NULL
         1 -- ????????????
         0 -- ????????????data
*/
int llist_travel2(LLIST *me, void *data)
{
	static LLIST *listhead = NULL;
	static node *cur = NULL;

	if(NULL == data)
		return -1;
	
	if(NULL != me){
		listhead = me;
		cur = listhead->head.next;
		memcpy(data, cur->data, listhead->size);
		return 0;
	}
	else
	{
		if(listhead == NULL || cur == NULL)
			return -2;
		cur = cur->next;
		if(cur == &(listhead->head)){
			// loop finished, end
			cur = NULL;
			listhead = NULL;
			return 1;
		}
		else{
			memcpy(data, cur->data, listhead->size);
			return 0;
		}
	}
	return 0;
}

void llist_destroy(LLIST *me)
{
	node *cur, *next;
	for(cur = me->head.next;cur != &me->head;cur = next){
		next = cur->next;
		#ifdef USE_POINTER_DATA
		free(cur->data);
		#endif
		free(cur);
	}
	free(me);
	return;
}


