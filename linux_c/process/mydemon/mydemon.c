#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


#define DBGERR(str,args...)   fprintf(stderr, str "\n",##args)
#define DBGINFO(str,args...)  fprintf(stdout, str "\n",##args)


int mydemon(void)
{
	fflush(NULL);
	pid_t pid = fork();
	if(pid < 0){
		DBGERR("fork failed");
		return -1;
	}
	else if(0 < pid){
		exit(0);
	}

	else{ //if(0 == pid){
		DBGINFO("fork success");
		int fd = open("/dev/null",O_RDWR);
		if(fd < 0){
			DBGERR("open error:%s",strerror(errno));
		}
#if 0
		dup2(fd, 0);
		dup2(fd, 1);
		dup2(fd, 2);
#endif
		if(fd > 2){
			close(fd);
		}
		setsid();
		return 0;
	}

}


#define FNAME "/tmp/out"

int main()
{
	mydemon();

	FILE *fp = fopen(FNAME, "w");
	if(NULL == fp){
		DBGERR("fopen error:%s",strerror(errno));
		exit(1);
	}

	for(int i=0;;i++)
	{
		fprintf(fp,"%d\n",i);
		fflush(fp);
		sleep(1);
	}

	fclose(fp);
	exit(0);

}
