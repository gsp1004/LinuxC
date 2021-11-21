#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#include"relayer.h"


#define TTY1 "/dev/tty9"
#define TTY2 "/dev/tty10"
#define TTY3 "/dev/tty11"
#define TTY4 "/dev/tty12"

int opentty(char * tty)
{
	int fd = open(tty, O_RDWR);
	if(fd < 0){
		perror("open()");
		exit(1);
	}
	write(fd, tty, strlen(tty));
	write(fd, "\n", 1);
	return fd;
}

int main()
{
	int fd1,fd2,fd3,fd4;
	int job1,job2;
	
	fd1 = opentty(TTY1);
	fd2 = opentty(TTY2);
	fd3 = opentty(TTY3);
	fd4 = opentty(TTY4);

	job1 = rel_addjob(fd1, fd2);
	if(job1<0){
		fprintf(stderr, "rel_addjob():%s\n", strerror(-job1));
		exit(1);
	}

	job2 = rel_addjob(fd3, fd4);
	if(job2<0){
		fprintf(stderr, "rel_addjob():%s\n", strerror(-job2));
		exit(1);
	}

	while(1){
		pause();
	}
	close(fd4);
	close(fd3);
	close(fd2);
	close(fd1);
	exit(0);
}



