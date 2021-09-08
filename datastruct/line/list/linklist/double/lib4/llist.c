#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"llist.h"
#include"gsp.h"

typedef struct llist_node_st
{
	struct llist_node_st *prev;
	struct llist_node_st *next;
	#ifdef USE_POINTER_DATA
	void *data;
	#else
	char data[0];  // only C99 support char arr[0]
	/* if not support C99, we can use char data[1]
	   this will only expend 4 more Bytes (not 1 Byte
	   because of the Memory alignment)
	*/
	
	#endif
}node;

typedef struct llist_head_st
{
	int size;
	struct llist_node_st head;
}LLIST;


LIST *llist_create(int initsize)
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
	return ret;
}

int llist_insert(LIST *m, const void *data, insert_mode mode)
{
	LLIST * me = m;
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
static node *find_(LIST *m, const void *key, llist_cmp *cmp)
{
	LLIST * me = m;
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
void * llist_find(LIST *m, const void *key, llist_cmp *cmp)
{
	LLIST * me = m;
	node * find_node;
	find_node = find_(me,key,cmp);
	if(find_node == NULL)
		return NULL;
	else
		return find_node->data;
}

/*对应数据直接删除
return: -1-- not found
		0 -- success
*/
int llist_delete(LIST *m, const void *key, llist_cmp *cmp)
{
	LLIST * me = m;
	node * find_node;
	find_node = find_(me,key,cmp);
	if(find_node == NULL)
		return -1;

	// 脱链
	find_node->prev->next = find_node->next;
	find_node->next->prev = find_node->prev;

	// 释放内存
	#ifdef USE_POINTER_DATA
	free(find_node->data);
	#endif
	free(find_node);
	return 0;
}

/*对应数据从列表中提取出来*/
int llist_fetch(LIST *m, const void *key, llist_cmp *cmp, void *data)
{
	LLIST * me = m;
	node * find_node;
	find_node = find_(me,key,cmp);
	if(find_node == NULL)
		return -1;

	// 脱链
	find_node->prev->next = find_node->next;
	find_node->next->prev = find_node->prev;

	// 数据回填
	if(NULL != data)
		memcpy(data, find_node->data, me->size);

	#ifdef USE_POINTER_DATA
	free(find_node->data);
	#endif
	free(find_node);
	return 0;
}

/*
要求用户提供回调函数用于处理用户数据，
因为我们不知道用户的数据结构
*/
void llist_travel(LIST *m, llist_op *func)
{
	LLIST * me = m;
	node * cur;
	for(cur = me->head.next; cur != &me->head; cur = cur->next)
	{
		func(cur->data);
	}
	return;
}


/*
函数功能：依次遍历链表中的data
使用说明：第一次使用必须传入参数1。后续需要便利链表的时候，参数1传入NULL
输入参数1：链表的头指针，如果为NULL，自动向后寻找下一个
输出参数2：返回找到的数据给用户
返回值：-1 -- data参数为NULL
        -2 -- 第一次使用时参数一为NULL
         1 -- 便利结束
         0 -- 成功找到data
*/
int llist_travel2(LIST *m, void *data)
{
	LLIST * me = m;
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

void llist_destroy(LIST *m)
{
	LLIST * me = m;
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


