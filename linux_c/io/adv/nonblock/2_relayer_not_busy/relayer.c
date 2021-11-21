#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<pthread.h>
#include<poll.h>

#include"relayer.h"


#define BUFSIZE (1024)

typedef enum{
	STATE_R,
	STATE_W,
	STATE_EX,
	STATE_T,
}fsm_state_e;


// relayer Finite State Machine sturct
typedef struct rel_fsm{
	int state;
	int sfd;
	int dfd;
	char buf[BUFSIZE];
	int len;
	char *errstr;
	int pos;
	int64_t count;
}rel_fsm_st;


// relayer job sturct
typedef struct{
	int state;
	int fd1;
	int fd2;
	rel_fsm_st fsm12, fsm21;
	int fd1_save, fd2_save;
	// struct timerval start,end;
}rel_job_st;

// global /////////////////////////////////////////////////
static rel_job_st* rel_job[REL_JOBMAX];
static pthread_mutex_t mut_rel_job = PTHREAD_MUTEX_INITIALIZER;
static pthread_once_t module_load_init_once = PTHREAD_ONCE_INIT;
static pthread_t tid_relayer;

int get_free_pos_unlocked()
{
	for(int i=0;i<REL_JOBMAX;i++){
		if(rel_job[i] == NULL){
			return i;
		}
	}
	return -1;
}

static void fsm_driver(rel_fsm_st *fsm)
{
	int ret;
	switch (fsm->state)
	{
		case STATE_R:
			fsm->len = read(fsm->sfd, fsm->buf, BUFSIZE);
			if(fsm->len == 0){
				fprintf(stderr, "read none, goto STATE_T!\n");
				fsm->state = STATE_T;
			}
			else if(fsm->len < 0){
				if(errno == EAGAIN){
					fsm->state = STATE_R;
				}
				else{
					fsm->errstr = "read()";
					fsm->state = STATE_EX;
				}
			}
			else{
				fsm->pos = 0;
				fsm->state = STATE_W;
			}
			break;
		case STATE_W:
			ret = write(fsm->dfd, fsm->buf + fsm->pos, fsm->len);
			if(ret < 0){
				if(errno == EAGAIN){
					fsm->state = STATE_W;
				}
				else{
					fsm->errstr = "write()";
					fsm->state = STATE_EX;
				}
			}
			else{
				fsm->pos += ret;
				fsm->len -= ret;
				if(fsm->len == 0){
					fsm->state = STATE_R;
				}
				else{
					fsm->state = STATE_W;
				}
			}
			break;
		case STATE_EX:
			perror(fsm->errstr);
			fsm->state = STATE_T;
			break;
		case STATE_T:
			// do sth
			break;
		default:
			fprintf(stderr, "error:fsm_driver(): unkonwn state %d\n", fsm->state);
			abort();
			break;

	}
}

static void* thr_relayer(void * args)
{
	int i, fds_num, ret;
	struct pollfd fds[REL_JOBMAX * 2];
#if 0
struct pollfd {
	int   fd;		  /* file descriptor */
	short events;	  /* requested events */
	short revents;	  /* returned events */
};
#endif

	while(1){
		fds_num = 0;
		// set all the interested fd
		for(i=0;i<REL_JOBMAX;i++){
			if(rel_job[i] != NULL){
				if(rel_job[i]->fsm12.state == STATE_R){
					fds[fds_num].fd = rel_job[i]->fsm12.sfd;
					fds[fds_num].events = POLLIN;
					fds_num++;
				}
				else if(rel_job[i]->fsm12.state == STATE_W){
					fds[fds_num].fd = rel_job[i]->fsm12.dfd;
					fds[fds_num].events = POLLOUT;
					fds_num++;
				}

				if(rel_job[i]->fsm21.state == STATE_R){
					fds[fds_num].fd = rel_job[i]->fsm21.sfd;
					fds[fds_num].events = POLLIN;
					fds_num++;
				}
				else if(rel_job[i]->fsm21.state == STATE_W){
					fds[fds_num].fd = rel_job[i]->fsm21.dfd;
					fds[fds_num].events = POLLOUT;
					fds_num++;
				}
			}
		}

		while(fds_num){
			ret = poll(fds, fds_num, -1);
			if(ret < 0){
				if(errno == EINTR){
					continue;
				}
				perror("poll()");
				exit(1);
			}
			else{
				//fprintf(stderr, "poll() ret = %d\n", ret);
				break;
			}
		}

		pthread_mutex_lock(&mut_rel_job);
		for(i=0;i<REL_JOBMAX;i++){
			if(rel_job[i] != NULL){
				if(rel_job[i]->state == STATE_RUNNING){
					if(rel_job[i]->fsm12.state != STATE_T){
						fsm_driver(&rel_job[i]->fsm12);
					}
					
					if(rel_job[i]->fsm21.state != STATE_T){
						fsm_driver(&rel_job[i]->fsm21);
					}

					if(rel_job[i]->fsm12.state == STATE_T && \
						rel_job[i]->fsm21.state == STATE_T){
						rel_job[i]->state = STATE_OVER;
					}
				}
			}
		}
		pthread_mutex_unlock(&mut_rel_job);
	}
	return NULL;
}

static void module_unload(void)
{
	pthread_cancel(tid_relayer);
	pthread_join(tid_relayer, NULL);
	return;
}


static void module_load(void)
{
	int err = pthread_create(&tid_relayer, NULL, thr_relayer, NULL);
	atexit(module_unload);
	if(err){
		fprintf(stderr, "pthred_create():%s\n",strerror(err));
		exit(1);
	}
	return;
}

int rel_addjob(int fd1, int fd2)
{
	rel_job_st *me;
	int pos;

	pthread_once(&module_load_init_once, module_load);

	pthread_mutex_lock(&mut_rel_job);
	pos = get_free_pos_unlocked();
	if(pos < 0){
		pthread_mutex_unlock(&mut_rel_job);
		return -ENOSPC;
	}
	pthread_mutex_unlock(&mut_rel_job);

	me = malloc(sizeof(*me));
	if(NULL == me){
		return -ENOMEM;
	}

	me->fd1 = fd1;
	me->fd2 = fd2;
	me->state = STATE_RUNNING;

	me->fd1_save = fcntl(me->fd1, F_GETFL);
	fcntl(me->fd1, F_SETFL, me->fd1_save|O_NONBLOCK);
	me->fd2_save = fcntl(me->fd2, F_GETFL);
	fcntl(me->fd2, F_SETFL, me->fd2_save|O_NONBLOCK);

	me->fsm12.sfd = me->fd1;
	me->fsm12.dfd = me->fd2;
	me->fsm12.state = STATE_R;

	me->fsm21.sfd = me->fd2;
	me->fsm21.dfd = me->fd1;
	me->fsm21.state = STATE_R;

	rel_job[pos] = me;
	return pos;
}

