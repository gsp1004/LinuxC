#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"nohead.h"
#include"gsp.h"

int main()
{
	list *nohead_list = NULL;
	score tmp;

	//srand((unsigned)time(NULL));
	srand(time(NULL));
	for(int i=0; i<7; i++)
	{
		tmp.id = i;
		snprintf(tmp.name,NAMESIZE,"stu%d",i);
		tmp.math = rand() % 100;
		tmp.chinese = rand() % 100;

		int err;
		if(err = list_insert(&nohead_list, &tmp))
		{
			printf("insert error %d,exit 1\n",err);
			exit(1);
		}
	}

	list_show(nohead_list);

	list_delete(&nohead_list);
	list_show(nohead_list);

	list *target_node = NULL;
	if(list_find(nohead_list, 2, &target_node))
	{
		DBGLINE("find node failed!");
	}
	else
	{
		printf("%d\t%s\t%d\t%d\n",target_node->data.id,target_node->data.name,target_node->data.math,target_node->data.chinese);
	}
	if(list_find(nohead_list, 9, &target_node))
	{
		DBGLINE("find node failed!");
	}
	list_destroy(nohead_list);
	exit(0);
}
