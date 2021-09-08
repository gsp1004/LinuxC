#ifndef LLIST_H__
#define LLIST_H__
/*
lib3: try to use object-oriented way to make a class
      to include all the method(the function) into 
      the struct(class)
*/
/*
  if we use #define USE_POINTER_DATA
  this is an old way which we must 
  malloc and free data's memery separately.
  the new way is to use char data[0] as a placeholder
  at the end of the struct and each time when we malloc
  the memery, we apply sizeof(head.size) more memery for the
  struct, then it is used to save the data
*/ 
//#define USE_POINTER_DATA  

typedef void llist_op(const void *data);
typedef int llist_cmp(const void *, const void *);


typedef enum insert_mode_e
{
	LLIST_FORWARD = 1,
	LLIST_BACKWARD,
}insert_mode;

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
	int (*insert)(LLIST *me, const void *data, insert_mode mode);
	void * (*find)(LLIST *me, const void *key, llist_cmp *cmp);
	int (*delete)(LLIST *me, const void *key, llist_cmp *cmp);
	int (*fetch)(LLIST *me, const void *key, llist_cmp *cmp, void *data);
	void (*travel)(LLIST *me, llist_op *func);
	int (*travel2)(LLIST *me, void *data);
}LLIST;

LLIST *llist_create(int initsize);

void llist_destroy(LLIST *me);

#endif
