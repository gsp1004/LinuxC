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
	printf("press 'ctrl+C' all the time and you will see the program not run 10s\n");
	return;
}


int main()
{
	usage();
	signal(SIGINT, int_handler);
	for(int i=0;i<10;i++){
		write(1, "*", 1);
		// man 3 sleep to see the return value of 'sleep' funtion
		if(sleep(2)){
			//perror("sleep():");
			fprintf(stderr, "errno = %d,sleep():%s\n", errno, strerror(errno));
		}
	}
	putchar('\n');
	return 0;
}
