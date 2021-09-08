#ifndef LLIST_H__
#define LLIST_H__
/*
lib4: hide our data struct
*/
typedef void LIST;
typedef void llist_op(const void *data);
typedef int llist_cmp(const void *, const void *);

typedef enum insert_mode_e
{
	LLIST_FORWARD = 1,
	LLIST_BACKWARD,
}insert_mode;

LIST *llist_create(int initsize);

int llist_insert(LIST *me, const void *data, insert_mode mode);

void * llist_find(LIST *me, const void *key, llist_cmp *cmp);

int llist_delete(LIST *me, const void *key, llist_cmp *cmp);

int llist_fetch(LIST *me, const void *key, llist_cmp *cmp, void *data);

void llist_travel(LIST *me, llist_op *func);

int llist_travel2(LIST *me, void *data);

void llist_destroy(LIST *me);

#endif
