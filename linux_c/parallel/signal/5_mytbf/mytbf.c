#include<signal.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include"mytbf.h"

#define MYTBF_MAX (1024)
#define MIN(X,Y) ((X) > (Y)? (Y):(X))
struct mytbf_st{
	int cps;
	int burst;
	int token;
	int pos;
};

static struct mytbf_st * job[MYTBF_MAX];
typedef void (*sighandler_t)(int);
static sighandler_t alrm_handler_save;

/** get_free_pos
 *  return the free position of job[array], if there is no free position, return -1;
 *  return: -1 -- there is no free position
 *          >=0 - the free position index
 */
static int get_free_pos(void)
{
	int i=0;
	for(i=0;i<MYTBF_MAX;i++){
		if(job[i] == NULL)
			return i;
	}
	return -1;
}

static void alrm_handler(int s)
{
	int i=0;
	alarm(1);
	for(i = 0; i < MYTBF_MAX; i++){
		if(job[i] != NULL){
			job[i]->token += job[i]->cps;
			if(job[i]->token > job[i]->burst){
				job[i]->token = job[i]->burst;
			}
		}
	}
}

static void module_unload(void)
{
	int i=0;
	signal(SIGALRM, alrm_handler_save);
	alarm(0);
	for(i = 0; i < MYTBF_MAX; i++){
		if(job[i]){
			free(job[i]);
		}
	}
}


static void module_load(void)
{
	alrm_handler_save = signal(SIGALRM, alrm_handler);
	alarm(1);
	atexit(module_unload);
}

mytbf_t *mytbf_init(int cps, int burst)
{
	static int flag = 1;
	struct mytbf_st *me;

	if(flag){
		module_load();
		flag = 0;
	}
	
	int pos = get_free_pos();
	if(pos<0){
		return NULL;
	}
	me = malloc(sizeof(*me));
	if(NULL == me)
		return me;

	me->token = 0;
	me->cps = cps;
	me->burst = burst;
	me->pos = pos;
	job[pos] = me;
	return me;
}

int mytbf_fetchtoken(mytbf_t *ptr, int size)
{
	struct mytbf_st *me = ptr;
	int ret = 0;
	if(size <= 0){
		return -EINVAL;
	}

	while(me->token <= 0){
		pause();
	}

	ret = MIN(me->token, size);
	me->token -= ret;
	return ret;
}

int mytbf_returntoken(mytbf_t *ptr, int size)
{
	struct mytbf_st *me = ptr;
	if(size <= 0){
		return -EINVAL;
	}
	me->token += size;
	if(me->token > me->burst){
		me->token = me->burst;
	}
	return size;
}

int mytbf_destory(mytbf_t *ptr)
{
	struct mytbf_st *me = ptr;
	job[me->pos] = NULL;
	free(ptr);
	return 0;
}

