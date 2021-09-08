#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<time.h>
#include"josephu.h"
#include"gsp.h"
/*
约瑟夫问题编程：
问题原型
1. 现有T个人围成一桌坐下，编号从1到T，从编号为1的人开始报数。 
2. 报数也从1开始，报到M的人离席，从离席者的下一位在座成员开始，继续从1开始报数。 
3. 复现这个过程（各成员的离席次序），或者求最后一个在座的成员编号。

代码中定义的
参与人数JOSE_NUM = 8
间隔人数SKIP_NUM = 3，也就是每间隔2个人出局一个
*/
#define JOSE_NUM  (8)
#define SKIP_NUM  (3)

int main(int argc, char *argv[])
{
	printf("argc = %d,argv[0] = %s\n\n", argc,argv[0]);
	list * me;

	int num = JOSE_NUM;
	if(1 < argc)
		num = atoi(argv[1]);

	me = jose_create(num);
	if(NULL == me)
		exit(1);
	jose_show(me);

	int delete_num = SKIP_NUM;
	if(2 < argc)
		delete_num = atoi(argv[2]);
	jose_kill(&me, delete_num);
	jose_show(me);

	return 0;
}
