#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<errno.h>
#include<string.h>


static void int_handler(int s)
{
	write(1, "!", 1);
}


void usage(void)
{
	printf("press 'ctrl+C', the singal will be reponsed later\n");
	return;
}

void block_signal(int signal)
{
	sigset_t s;
	sigemptyset(&s);
	sigaddset(&s, signal);
	sigprocmask(SIG_BLOCK, &s, NULL);
}

void unblock_signal(int signal)
{
	sigset_t s;
	sigemptyset(&s);
	sigaddset(&s, signal);
	sigprocmask(SIG_UNBLOCK, &s, NULL);
}

//save the signal status, when finish the call, revert it
typedef enum
{
	ENV_SAVE,
	ENV_REVERT,
}env_op_e;

void save_env(env_op_e env)
{
	static sigset_t s;
	if(ENV_SAVE == env){
		sigset_t tmp;
		sigemptyset(&tmp);
		sigemptyset(&s);
		sigprocmask(SIG_UNBLOCK, &tmp, &s);  // save env signal to s
	}
	else{
		sigprocmask(SIG_SETMASK, &s, NULL);
	}

}
/*
   typedef void (*sighandler_t)(int);

   sighandler_t signal(int signum, sighandler_t handler);


 */

int main()
{
	usage();

	void (*sighandler_before)(int);
	sighandler_before = signal(SIGINT, int_handler);

	save_env(ENV_SAVE);

	for(int j=0;j<5;j++){
		// block signal
		block_signal(SIGINT);
		for(int i=0;i<3;i++){
			write(1, "*", 1);
			// man 3 sleep to see the return value of 'sleep' funtion
			if(sleep(2)){
				fprintf(stderr, "errno = %d,sleep():%s\n", errno, strerror(errno));
			}
		}
		unblock_signal(SIGINT);
		write(1, "\n", 1);
	}
	putchar('\n');

	save_env(ENV_REVERT);
	signal(SIGINT, sighandler_before);

	return 0;
}
