#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>


/**
 * main.c - a sample of search tree.
 *
 * use more memery and speed less time to search.
 * the kernel is the amazing tree with 26 child node.
 * reference: https://www.bilibili.com/video/BV18p4y167Md?p=115  13:31
*/
#define LOGFILE "./log"

#define DESCSIZE (256)
#define KEYSIZE (32)
#define BUFSIZE (DESCSIZE+KEYSIZE)
#define CHARNUM (26)


typedef struct node_st
{
	struct node_st *ch[CHARNUM];
	char desc[DESCSIZE];
}tree_node;

/**
 * get_word - read one line from file and extract key & desc
 *
 * return: 0 - success
 *        -1 - failed
*/
int get_word(FILE *fp, char *key, char *desc)
{
	char buf[BUFSIZE];
	char *retp;
	int i,j;

	retp = fgets(buf,sizeof(buf),fp);
	if(NULL == retp){
		//printf("fgets failed\n");
		return -1;
	}


	for(i=0;i<KEYSIZE-1 && buf[i]!=':'; i++){
		key[i] = buf[i];
	}
	key[i] = '\0';
	
	++i;
	
	for(j=0;j<DESCSIZE-1 && buf[i]!='\0';j++,i++){
		desc[j] = buf[i];
	}
	desc[j] = '\0';


	return 0;
}


tree_node *newnode(void)
{
	tree_node *node;
	node = malloc(sizeof(*node));
	if(NULL == node)
		return NULL;

	// INIT
	memset(node->desc,0,DESCSIZE);
	for(int i=0;i<CHARNUM;i++){
		node->ch[i] = NULL;
	}
	return node;
}


/**
 * insert - 
*/
int insert(tree_node **root, char *key, char *desc)
{
	if(NULL == *root){
		*root = newnode();
		if(NULL == *root){
			printf("malloc error\n");
			return -1;
		}

	}

	if(*key == 0){
		strcpy((*root)->desc,desc);
		return 0;
	}
	
	return insert(&((*root)->ch[*key-'a']),key+1,desc);
}

/*

 * return: the desc
*/
char *find(tree_node *root, char *key)
{
	if(NULL == root)
		return NULL;
	if(*key == '\0')
		return root->desc;

	return find(root->ch[*key-'a'],key+1);
}


int main()
{
	tree_node *tree=NULL;
	FILE *fp;
	char desc[DESCSIZE],key[KEYSIZE];
	char *datap;
	fp = fopen(LOGFILE,"r");
	if(NULL == fp)
		exit(1);

	while(1){
		if(get_word(fp, key, desc)){
			printf("get_word finished\n");
			break;
		}
		insert(&tree,key,desc);
	}

	datap = find(tree, "ant1");
	if(NULL == datap)
		printf("can not find\n");
	else
		puts(datap);

	fclose(fp);
	exit(0);
}
