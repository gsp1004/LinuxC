#ifndef RELAYER_H__
#define RELAYER_H__

#define REL_JOBMAX (10000)

typedef enum
{
	STATE_RUNNING=1,
	STATE_CANCELED,
	STATE_OVER
}rel_state_e;

typedef struct rel_stat{
	rel_state_e state;
	int fd1;
	int fd2;
	int64_t count12, count21;
}rel_stat_st;

int rel_addjob(int fd1, int fd2);
/* return: >= 0			success. return the ID of task
           == -EINVAL	bad param
           == -ENOSPC	task full
           == -ENOMEM	malloc failed
*/

#if 0
int rel_canceljob(int fd);
/* return: >= 0			success. cancel succeed
           == -EINVAL	bad param
           == -EBUSY	task has been canceld before
*/

int rel_waitjob(int id, rel_stat_st *stat);
/* return: >= 0			success. task has been canceled and return status code
           == -EINVAL	bad param
*/

int rel_getstatjob(int id, rel_stat_st *stat);
/* return: >= 0			success. get task status code success
           == -EINVAL	bad param
*/
#endif

#endif