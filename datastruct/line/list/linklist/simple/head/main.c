#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"list.h"
#include"gsp.h"

int main()
{
	list *l;
	datatype arr[] = {1,11,2,22,3,33,4,0};
	int i;

	l = list_create();
	if(NULL == l)
		exit(1);

	for(i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{


		//if(list_insert_at(l, 0, &arr[i]) != 0)  // test this function
		if(list_insert_order(l, &arr[i]) != 0)  // test function
		{
			fprintf(stderr, "error in (%s %s %d)!\n",__FILE__, __FUNCTION__, __LINE__);
		}
	}
	list_display(l);

	list_delete_at(l, 2);
	list_display(l);

	list_delete(l, &arr[7]);
	list_display(l);

	list_destory(l);
	exit(0);
}
