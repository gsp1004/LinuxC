#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>

#include"sqstack.h"
#include"queue.h"

/**
 * main.c - this code use stack and queue to calculate
 * how much time do we speed when the number in queue
 * is in original order.
 * There are 3 stacks, 1 queue.
 * 3 stacks: 
 *          1 hour -- hold 11 ball for most
 *          5 min  -- hold 11 ball for most
 *          1 min  -- hold 4 ball for most
 * 1 queue: has 27 ball and each ball has a number (1..27)
*/

#define MAX1MIN (4)
#define MAX5MIN (11)
#define MAX1HOUR  (23 + 24)
#define NR_BALL (MAX1MIN + MAX5MIN + MAX1HOUR + 1)

/* stisfull - test whether stack is full
 * @st:  the stack
 * @num: the stack max number
 * return: 0 -- not full
 *         1 -- full
*/
int stisfull(sqstack *st, int num)
{
	return (st_number(st) == num);
}

/* clear_stack - put all stack member to queue
 * @st:  the stack
 * @qu:  the queue
*/
void clear_stack(sqstack *st, queue *qu)
{
	while(!st_isempty(st)){
		stackdatatype data;
		st_pop(st, &data);
		qu_enqueue(qu, &data);
	}
}

/* put_ball_into_stack - try to put one ball into stack
 * @st:       a pointer to sqstack * arr, the arr save all the stack in order
 * @stackmax: a pointer to int arr, the arr save all the stack max value in order
 * @i:        the index, means which stack should we try
 * @stacknum: the number of stack in sqstack * arr
 * @ball:     a pointer to the ball
 * @qu:       the queue
 *
 * return:    >=0 -- put the ball into (the return number) stack
 *             -1 -- all stack full, need to check queue
 */
int put_ball_into_stack(sqstack ** st, int *stackmax, int i, int stacknum, int *ball, queue *qu)
{
	if(i >= stacknum){
		//qu_travel(qu);
		qu_enqueue(qu, ball);
		//printf("final ball is %d\n", *ball);
		//qu_travel(qu);
		return -1;
	}

	if(!stisfull(st[i], stackmax[i])){
		st_push(st[i], ball);
		return 0;
	}
	else{
		clear_stack(st[i], qu);
		// TODO:recursive
		return put_ball_into_stack(st, stackmax, i+1, stacknum, ball, qu);
	}
}

/* queue_isInOrder - check whether the queue member is in order
 * return: 1 -- in order
 *         0 -- not in order
*/
int queue_isInOrder(queue *qu, int ballnum)
{
	queuedatatype tmp;
	int ret = 1;
	//printf("head1\t");
	for(int i = 1; i <= ballnum; ++ i){
		qu_dequeue(qu, &tmp);
		//printf("%d\t",tmp);
		if(ret && tmp != i)
			ret = 0;
		qu_enqueue(qu, &tmp);
	}
	//printf("tail111\n");
	return ret;
}


int main()
{
	queue *qu;
	sqstack *st_1min, *st_5min, *st_1hour;
	queuedatatype ball;
	qu = qu_create();
	if(NULL ==qu)
		exit(1);

	st_1min = st_create();
	if(NULL == st_1min)
		exit(1);
	st_5min = st_create();
	if(NULL == st_5min)
		exit(1);
	st_1hour = st_create();
	if(NULL == st_1hour)
		exit(1);

	sqstack * starr[] = {st_1min, st_5min, st_1hour};
	int stmax[] = {MAX1MIN, MAX5MIN, MAX1HOUR};
	

	// init ball
	for(int i=1;i<=NR_BALL;i++){
		qu_enqueue(qu, &i);
	}

	// get 1 ball out, try to put into stack
	long long time = 0;
	while(1){
		qu_dequeue(qu, &ball);
		++time;
		int ret;
		if(-1 == (ret = put_ball_into_stack(starr, stmax, 0, sizeof(stmax)/sizeof(stmax[0]), &ball, qu)))
		{
			if(queue_isInOrder(qu, NR_BALL)){
				printf("cause time %lld min\n", time);
				break;
			}
			else if(0)  // show the full
			{
				printf("\nfinish one loop, show queue, time = %lld\n", time);
				//qu_travel(qu);
				//usleep(100 * 1000);
			}
		}
	}

	

	qu_destroy(qu);
	st_destroy(st_1min);
	st_destroy(st_5min);
	st_destroy(st_1hour);

	exit(0);
}
