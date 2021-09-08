#ifndef SQLIST_H__
#define SQLIST_H__

#define DATASIZE 1024

typedef int datatype;

typedef struct node_st
{
    datatype data[DATASIZE];
    int last;  // -1 means no member
}sqlist;


sqlist *sqlist_create();

void sqlist_create1(sqlist **);  // fail to return NULL

int sqlist_destroy(sqlist *);  // return 0 for success

int sqlist_insert(sqlist *, int i, datatype *);  // use pointer is better,  return 0 for success

int sqlist_delete(sqlist *, int i);

int sqlist_find(sqlist *, datatype *);

int sqlist_isempty(sqlist *);

int sqlist_setempty(sqlist *);

int sqlist_getnum(sqlist *);

void sqlist_display(sqlist *);

int sqlist_union(sqlist *, sqlist *);

#endif
