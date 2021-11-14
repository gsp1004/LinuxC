#include<signal.h>
#include<stdio.h>
#include<string.h>

#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#include"mytbf.h"

#define MYTBF_MAX (1024)
#define MIN(X,Y) ((X) > (Y)? (Y):(X))
struct mytbf_st{
	int cps;
	int burst;
	int token;
	int pos;
	pthread_mutex_t mut;
	pthread_cond_t cond;
};

static struct mytbf_st * job[MYTBF_MAX];
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;
pthread_t tid;

//typedef void (*sighandler_t)(int);
//static sighandler_t alrm_handler_save;

/** get_free_pos_unlocked
 *  return the free position of job[array], if there is no free position, return -1;
 *  return: -1 -- there is no free position
 *          >=0 - the free position index
 */
static int get_free_pos_unlocked(void)
{
	int i=0;
	for(i=0;i<MYTBF_MAX;i++){
		if(job[i] == NULL)
			return i;
	}
	return -1;
}

static void* alrm_handler(void * n)
{
	int i=0;
	while(1){
		pthread_mutex_lock(&mut_job);
		for(i = 0; i < MYTBF_MAX; i++){
			if(job[i] != NULL){
				pthread_mutex_lock(&job[i]->mut);
				job[i]->token += job[i]->cps;
				if(job[i]->token > job[i]->burst){
					job[i]->token = job[i]->burst;
				}
				//pthread_cond_broadcast(&job[i]->cond);
				pthread_cond_signal(&job[i]->cond);
				pthread_mutex_unlock(&job[i]->mut);
			}
		}
		pthread_mutex_unlock(&mut_job);
		sleep(1);  // usleep();
	}
	return NULL;
}

static void module_unload(void)
{
	int i=0;
	//signal(SIGALRM, alrm_handler_save);
	//alarm(0);

	pthread_cancel(tid);
	pthread_join(tid, NULL);

	for(i = 0; i < MYTBF_MAX; i++){
		if(job[i]){
			mytbf_destroy(job[i]);
		}
	}
	pthread_mutex_destroy(&mut_job);
}

static void module_load(void)
{
	//alrm_handler_save = signal(SIGALRM, alrm_handler);
	//alarm(1);

	int err = pthread_create(&tid, NULL, alrm_handler, NULL);
	if(err){
		fprintf(stderr, "pthread_create(): %s\n", strerror(err));
		exit(1);
	}
	
	atexit(module_unload);
}

mytbf_t *mytbf_init(int cps, int burst)
{
	//static int inited = 0;
	static pthread_once_t init_once = PTHREAD_ONCE_INIT;
	struct mytbf_st *me;

/*
	lock();
	if(!inited){
		module_load();
		inited = 1;
	}
	unlock();
*/
	pthread_once(&init_once, module_load);

	me = malloc(sizeof(*me));
	if(NULL == me)
		return me;

	me->token = 0;
	me->cps = cps;
	me->burst = burst;
	pthread_mutex_init(&(me->mut), NULL);
	pthread_cond_init(&(me->cond), NULL);

	pthread_mutex_lock(&mut_job);
	int pos = get_free_pos_unlocked();
	if(pos<0){
		pthread_mutex_unlock(&mut_job);
		free(me);
		return NULL;
	}
	me->pos = pos;
	job[pos] = me;
	pthread_mutex_unlock(&mut_job);
	return me;
}

int mytbf_fetchtoken(mytbf_t *ptr, int size)
{
	struct mytbf_st *me = ptr;
	int ret = 0;
	if(size <= 0){
		return -EINVAL;
	}

	pthread_mutex_lock(&me->mut);
	while(me->token <= 0){  // busy
	#if 0  // query method
		pthread_mutex_unlock(&me->mut);
		sched_yield();
		pthread_mutex_lock(&me->mut);
	#else  // notify method
		pthread_cond_wait(&me->cond, &me->mut);
	#endif
	}

	ret = MIN(me->token, size);
	me->token -= ret;
	pthread_mutex_unlock(&me->mut);
	return ret;
}

int mytbf_returntoken(mytbf_t *ptr, int size)
{
	struct mytbf_st *me = ptr;
	if(size <= 0){
		return -EINVAL;
	}
	pthread_mutex_lock(&me->mut);
	me->token += size;
	if(me->token > me->burst){
		me->token = me->burst;
	}
	// if mute-user(eg:A and B) use the same tbf, when B return token, 
	// wake A up to see wthether the token is enough, so add pthread_cond_signal here
	pthread_cond_signal(&me->cond);
	pthread_mutex_unlock(&me->mut);
	return size;
}

int mytbf_destroy(mytbf_t *ptr)
{
	struct mytbf_st *me = ptr;
	pthread_mutex_lock(&mut_job);
	job[me->pos] = NULL;
	pthread_mutex_unlock(&mut_job);
	pthread_mutex_destroy(&me->mut);
	pthread_cond_destroy(&me->cond);
	free(ptr);
	return 0;
}

