#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>

#include<queue.h>  // in ~/gsp_lib/include

/**
 * main.c - 
*/

#define NAMESIZE (32)

typedef struct score_st
{
	int id;
	char name[NAMESIZE];
	int math;
}score;

typedef struct node_st
{
	score data;
	struct node_st *l, *r;
}tree_node;

void printf_s(score *d)
{
	printf("%d %s %d\n",d->id,d->name,d->math);
}

enum travel_mode
{
	PREORDER = 0,  // xianxu 
	INORDER,       // zhongxu
	POSTORDER,     // houxu
	TRAVEL_MODE_BOTTOM
};

void travel(tree_node *root, enum travel_mode mode)
{
	if(mode < PREORDER || mode >=TRAVEL_MODE_BOTTOM){
		return;
	}

	if(root == NULL)
		return;
	if(PREORDER == mode){ printf_s(&root->data);}
	travel(root->l, mode);
	if(INORDER == mode){ printf_s(&root->data);}
	travel(root->r, mode);
	if(POSTORDER == mode){ printf_s(&root->data);}
}

/** 
 * travel_level - travel the tree by level, by queue
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
*/
#if 0
void travel_level(tree_node *root)
{
	head_node * qu;
	tree_node tmp;
	qu = qu_create(sizeof(tree_node));
	if(NULL == qu)
		return;
	qu_enqueue(qu, root);  // to debug
	while(!qu_isempty(qu)){
		qu_dequeue(qu, &tmp);
		printf_s(& tmp.data);
		if(tmp.l)
			qu_enqueue(qu, tmp.l);
		if(tmp.r)
			qu_enqueue(qu, tmp.r);
	}
	qu_clear(qu);
}
#else
void travel_level(tree_node *root)
{
	head_node *qu;
	tree_node *tmp;
	qu = qu_create(sizeof(tree_node *));
	if(NULL == qu)
		return;
	qu_enqueue(qu, &root);  // to debug
	while(!qu_isempty(qu)){
		qu_dequeue(qu, &tmp);
		printf_s(& tmp->data);
		if(tmp->l)
			qu_enqueue(qu,& tmp->l);
		if(tmp->r)
			qu_enqueue(qu,& tmp->r);
	}
	qu_clear(qu);
}
#endif

/*
 * table - (ROOT()())
*/
void table_(tree_node *root)
{
	if(NULL == root)
		return;
	//printf("(");
	printf("%d",root->data.id);
	printf("(");
	table_(root->l);
	printf(")");
	printf("(");
	table_(root->r);
	printf(")");
	//printf(")");
}

void table(tree_node *root)
{
	if(NULL == root)
		return;
	printf("(");
	table_(root);
	printf(")");
}

/**
 * insert - insert the data to the tree, by recursion
 * bigger to right, others to left
 * @root: pointer to the tree
 * @data: pointer to the data
*/
int insert(tree_node **root, score *data)
{
	tree_node *tmp;
	if(NULL == root || NULL == data)
		return -1;

	if(NULL == *root)
	{
		tmp = malloc(sizeof(tree_node));
		if(NULL == tmp)
			return -2;
		tmp->l = NULL;
		tmp->r = NULL;
		memcpy(&tmp->data, data, sizeof(*data));
		*root = tmp;
		return 0;
	}
	else
	{
		if(data->id > (*root)->data.id )
		{
			return insert( &((*root)->r), data);
		}
		else{
			return insert( &((*root)->l), data);
		}
	}
}

/* find_ -- for you really need to get the pointer to the node
 * @root: the pointer to the tree, the root node must need it
 * @id:   the key
 * return: the pointer to the node , or NULL for fail
*/
tree_node **find_(tree_node **root, int id)
{
	if(NULL == root || NULL == *root)
		return NULL;
	if(id == (*root)->data.id){
		return root;
	}
	else{
		if(id > (*root)->data.id)
			return find_(&((*root)->r), id);
		else
			return find_(&((*root)->l), id);
	}
}

/* find -- get the target node
 * @root: the tree
 * @id:   the key
 * return: the pointer to the node , or NULL for fail
*/
tree_node *find(tree_node *root, int id)
{
	tree_node ** node = find_(&root, id);
	if(NULL == node)
		return NULL;
	return *node;
}

/** draw_ - draw the whole tree by recursion, 
 *          draw right, then mid, finally left
 *  @root: the tree
 *  @level: which level to darw
*/
void draw_(tree_node *root, int level)
{
	int i;
	if(NULL == root)
		return;
	if(NULL != root->r){
		draw_(root->r, level + 1);
		for(i=0; i<level; i++)
		{
			printf("  ");
		}
		printf(" /\n");
	}

	for(i=0; i<level; i++)
	{
		printf("  ");
	}
	printf_s(&root->data);
	if(NULL != root->l){
		for(i=0; i<level; i++)
		{
			printf("  ");
		}
		printf(" \\\n");
		draw_(root->l, level + 1);
	}
}

void draw(tree_node *root)
{
	draw_(root, 0);
	//printf("\ndraw finished, wait for char---------\n");
	//getchar();
}

/** get_num - get number of the tree's node
 *  @root: the tree
*/
static int get_num(tree_node *root)
{
	if(NULL == root)
		return 0;

	return get_num(root->l) + 1 + get_num(root->r);
}


tree_node *find_min(tree_node *root)
{
	if(NULL == root)
		return NULL;
#if 0  // use reverse 
	if(NULL == root->l)
		return root;
	else
		return find_min(root->l);
#else  // use for loop
	tree_node * tmp;
	for(tmp = root;tmp->l != NULL;tmp = tmp->l);
	return tmp;
#endif
}

tree_node *find_max(tree_node *root)
{
	if(NULL == root)
		return NULL;
#if 0  // use reverse 
	if(NULL == root->r)
		return root;
	else
		return find_min(root->r);
#else  // use for loop
	tree_node * tmp;
	for(tmp = root;tmp->r != NULL;tmp = tmp->r);
	return tmp;
#endif
}

static void turn_left(tree_node **root)
{
	if(NULL == root || *root == NULL) return;
	tree_node *old_top = *root;
	*root = old_top->r;
	old_top->r = NULL;

	find_min(*root)->l = old_top;
}

static void turn_right(tree_node **root)
{
	if(NULL == root || *root == NULL) return;
	tree_node *cur = *root;
	*root = cur->l;
	cur->l = NULL;

	find_max(*root)->r = cur;
}


/** balance - balance the tree until number of 
 *  left - right is no more than 1
 *  @root: pointer to the tree
*/
void balance(tree_node **root)
{
	int sub;
	if(NULL == root|| NULL == *root){
		//printf("quit  balance root = %x; *root = %x\n", root, *root);
		return;
	}
	
	while(1)
	{
		// quit condition
		sub = get_num((*root)->l) - get_num((*root)->r);
		if(sub >=-1 && sub <= 1){
			break;
		}
		if(sub < -1){
			turn_left(root);
			//draw(*root);  // dbg
		}
		else{
			turn_right(root);
			//draw(*root);  // dbg
		}
	}

	balance(&(*root)->l);
	balance(&(*root)->r);
}


/* del_ -- this function will delete the node from tree and 
 *         move the subtree whose node is more to the hole.
 *         you must make sure the node is exist
 * @root: the node we need to delete
*/
static void del_(tree_node **root)
{
	if(NULL == root || NULL == *root)
		return;
	tree_node *del_node = *root;
	if(NULL == del_node->l && NULL == del_node->r){
		*root = NULL;
	}
	else if(get_num(del_node->l) > get_num(del_node->r)){
		*root = del_node->l;
		find_max(*root)->r = del_node->r;
	}
	else{
		*root = del_node->r;
		find_min(*root)->l = del_node->l;
	}
	free(del_node);
}

/* del -- delete a member from the tree
 * @root: the tree
 * @key:  according to the key to match the member
 * @func: how to match the key and member, return 1 is match
 * return: 0 -- success
 *        -1 -- 
*/
int del(tree_node **root, int id)
{
	tree_node ** node = find_(root, id);
	if(NULL == node)
		return -1;
	del_(node);
	return 0;
}

int main()
{
	//int arr[] = {1,2,3,7,6,5,4,8,9,0};
	int arr[] = {1,2,3,7,6,5,9,8,4};
	int i;
	tree_node *tree = NULL;
	score tmp;
	

	for(i = 0; i < sizeof(arr)/sizeof(*arr);i++)
	{
		tmp.id = arr[i];
		snprintf(tmp.name, NAMESIZE, "stu%d", arr[i]);
		tmp.math = rand()%100;
		insert(&tree, &tmp);
	}

#if 0// test insert and find function
	printf("find test start---------\n");
	tree_node *find_score_node;
	int tmpid = 2;
	find_score_node = find(tree, tmpid);
	if(NULL == find_score_node)
		printf("Not find id %d!\n",tmpid);
	else{
		printf_s(& find_score_node->data);
	}
	printf("find test over---------\n");	
#endif

#if 1  // test balance
	printf("balance test start--------------------\n");
	draw(tree);
	balance(&tree);
	printf("after balance--------------------\n");
	draw(tree);
	printf("balance test over--------------------\n");
#endif

#if 0  // test balance
	printf("del test start--------------------\n");
	int delid = 5;
	printf("del id = %d\n", delid);
	del(&tree, delid);
	draw(tree);
	printf("del test over--------------------\n");
#endif

#if 0  // test travel pre/in/post order
	printf("pre --------------------\n");
	travel(tree, PREORDER);
	printf("in  --------------------\n");
	travel(tree, INORDER);
	printf("post--------------------\n");
	travel(tree, POSTORDER);
#endif

#if 0  // test travel by level
	printf("test travel by level start--------------------\n");
	travel_level(tree);
	printf("test travel by level over --------------------\n");
#endif

	table(tree);
	printf("\n");

	exit(0);
}
