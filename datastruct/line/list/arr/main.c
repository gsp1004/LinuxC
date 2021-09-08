#include<stdio.h>
#include<stdlib.h>
#include"sqlist.h"



int main()
{
	sqlist *list;
	datatype arr[] = {1,2,3,4,5};
	datatype arr1[] = {11,22,33,44,55};
	int i,err;

	//list = sqlist_create();
	sqlist_create1(&list);
	if(NULL == list)
	{
		fprintf(stderr, "sqlist_create failed!\n");
		exit(1);
	}

	for(i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
	{
		if( (err = sqlist_insert(list, 0, &arr[i])) != 0)
		{
			if(-1 == err)
				fprintf(stderr, "insert failed, arr is full!\n");
			else if(-2 == err)
				fprintf(stderr, "insert failed, bad insert position!\n");
			else
				fprintf(stderr, "insert failed, unknown error!\n");
			exit(1);
		}
	}


//
	sqlist *list1;
	sqlist_create1(&list1);
	if(NULL == list1)
	{
		fprintf(stderr, "sqlist_create failed!\n");
		exit(1);
	}

	for(i=0; i<sizeof(arr1)/sizeof(arr1[0]); i++)
	{
		if( (err = sqlist_insert(list1, i, &arr1[i])) != 0)
		{
			if(-1 == err)
				fprintf(stderr, "insert failed, arr is full!\n");
			else if(-2 == err)
				fprintf(stderr, "insert failed, bad insert position!\n");
			else
				fprintf(stderr, "insert failed, unknown error!\n");
			exit(1);
		}
	}
//

	sqlist_display(list);
	sqlist_display(list1);

	sqlist_delete(list,1);

	sqlist_display(list);
	sqlist_display(list1);
	sqlist_union(list, list1);

	sqlist_display(list);
	sqlist_destroy(list);
	sqlist_destroy(list1);
	exit(0);
}
