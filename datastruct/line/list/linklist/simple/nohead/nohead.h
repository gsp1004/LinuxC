#ifndef NOHEAD_H__
#define NOHEAD_H__

#define NAMESIZE 32

/*students score data*/
typedef struct score_st
{
	int id;
	char name[NAMESIZE];
	int math;
	int chinese;
}score;

// LINK STRUCT
typedef struct node_st
{
	score data;
	struct node_st *next;
}list;

typedef score datatype;


int list_insert(list **me, datatype *data);

void list_show(list *me);

int list_delete(list **me);

int list_find(list * const me, int id, list **res);

void list_destroy(list * me);
#endif
