#include<stdio.h>
#include<stdlib.h>

#include"sqstack.h"

int main()
{
	datatype arr[] = {12,51,365,0,325};
	datatype data;
	sqstack * st;
	st = st_create();
	if(NULL == st)
		exit(1);

	for(int i=0; i< sizeof(arr)/sizeof(arr[0]); i ++)
		st_push(st, &arr[i]);

	st_travel(st);

	while(st_pop(st, &data) == 0){
		printf("pop data %d\n", data);
	}
	printf("pop finished\n");

#if 0 // push when full test
	datatype tmp = 1;
	if(st_push(st,&tmp))
		printf("Can not push\n");
	else
		st_travel(st);
#endif

	st_destroy(st);
	exit(0);
}
