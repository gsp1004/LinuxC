#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include<pthread.h>
#include <string.h>

#include"mypipe.h"

#define NEXT(a) ((a) == (PIPESIZE-1)?0,(a)+1)

typedef struct mypipe{
	int head;
	int tail;
	char data[PIPESIZE];
	int datasize;
	int count_rd;
	int count_wr;
	pthread_mutex_t mut;
	pthread_cond_t cond;
}mypipe_st;

mypipe_t *mypipe_init(void)
{
	mypipe_st *me;
	me = malloc(sizeof(*me));
	if(NULL == me){
		return NULL;
	}

	me->head = 0;
	me->tail = 0;
	me->datasize = 0;
	me->count_rd = 0;
	me->count_wr = 0;
	pthread_mutex_init(&me->mut, NULL);
	pthread_cond_init(&me->cond, NULL);
	return me;
}


int mypipe_register(mypipe_t *p, int opmap)
{
	mypipe_st *me = p;

	if(opmap & (MYPIPE_READ|MYPIPE_WRITE) == 0){
		return -1;
	}
	
	pthread_mutex_lock(&me->mut);
	if(opmap & MYPIPE_READ){
		me->count_rd++;
	}
	if(opmap & MYPIPE_WRITE){
		me->count_wr++;
	}
	pthread_cond_broadcast(&me->cond);
	
	while(me->count_rd <= 0 || me->count_wr <= 0){
		pthread_cond_wait(&me->cond, &me->mut);
	}
	pthread_mutex_unlock(&me->mut);
	return 0;
}


int mypipe_unregister(mypipe_t *p, int opmap)
{
	mypipe_st *me = p;

	if(opmap & (MYPIPE_READ|MYPIPE_WRITE) == 0){
		return -1;
	}
	
	pthread_mutex_lock(&me->mut);
	if(opmap & MYPIPE_READ){
		me->count_rd--;
	}
	if(opmap & MYPIPE_WRITE){
		me->count_wr--;
	}
	pthread_cond_broadcast(&me->cond);
	
	pthread_mutex_unlock(&me->mut);
	return 0;
}


static int mypipe_readbyte_unlocked(mypipe_st *me, char *datap)
{
	if(me->datasize <= 0)
		return -1;

	*datap = me->data[me->head];
	me->head = NEXT(me->head);
	me->datasize--;
	return 0;
}


static int mypipe_writebyte_unlocked(mypipe_st *me, char *datap)
{
	if(me->datasize >= PIPESIZE)
		return -1;

	me->data[me->tail] = *datap;
	me->tail = NEXT(me->tail);
	me->datasize++;	
	return 0;
}

/* mypipe_read: read [count] char from [p] to [buf]
attention:		1. if pipe is empty but there is writer, need to wait
				2. if pipe is empty and no writer, return 0 immediately
return:			 0 -- can not read
				>0 -- how many chars you read
*/
int mypipe_read(mypipe_t *p, void *buf, size_t count)
{
	mypipe_st *me = p;
	int i;
	pthread_mutex_lock(&me->mut);

	/*1. if pipe is empty but there is writer, need to wait*/
	while(me->datasize <= 0 && me->count_wr > 0){
		pthread_cond_wait(&me->cond, &me->mut);
	}

	/*2. if pipe is empty and no writer, return 0 immediately*/
	if(me->datasize <= 0 && me->count_wr == 0){
		pthread_mutex_unlock(&me->mut);
		return 0;
	}

	// get buf
	for(i = 0; i<count; i++){
		if(mypipe_readbyte_unlocked(me, buf+i) != 0){
			break;
		}
	}
	pthread_cond_broadcast(&me->cond);  // call those who wait to write
	pthread_mutex_unlock(&me->mut);
	return i;
}

/* mypipe_write: write [count] char from [buf] to [p]
attention:		1. if pipe is full but there is reader, need to wait
				2. if pipe is full and no reader, return 0 immediately
return:			 0 -- can not write
				>0 -- how many chars you write
*/
int mypipe_write(mypipe_t *p, const void *buf, size_t count)
{
	mypipe_st *me = p;
	int i;
	pthread_mutex_lock(&me->mut);

	/*1. if pipe is full but there is reader, need to wait*/
	while(me->datasize >= PIPESIZE && me->count_rd > 0){
		pthread_cond_wait(&me->cond, &me->mut);
	}

	/*2. if pipe is full and no reader, return 0 immediately*/
	if(me->datasize >= PIPESIZE && me->count_rd == 0){
		pthread_mutex_unlock(&me->mut);
		return 0;
	}

	// send buf to me
	for(i = 0; i<count; i++){
		if(mypipe_writebyte_unlocked(me, buf+i) != 0){
			break;
		}
	}
	pthread_cond_broadcast(&me->cond);
	pthread_mutex_unlock(&me->mut);
	return i;
}


int mypipe_destroy(mypipe_t *p)
{
	mypipe_st *me = p;
	pthread_mutex_destroy(&me->mut);
	pthread_cond_destroy(&me->cond);
	free(ptr);

	return 0;
}


