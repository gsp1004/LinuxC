#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>

#if 0
#define DBGERR(str,args...)   fprintf(stderr, str "\n",##args)
#define DBGINFO(str,args...)  fprintf(stdout, str "\n",##args)
#elif 1 // use syslog
#define DBGERR(str,args...)   syslog(LOG_ERR , str,##args)
#define DBGINFO(str,args...)  syslog(LOG_INFO, str,##args)
#endif
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

		dup2(fd, 0);
		dup2(fd, 1);
		dup2(fd, 2);

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

	openlog("mydemon",LOG_PID,LOG_DAEMON);

	if(mydemon()){
		DBGERR("mydaemon failed!");
	}else{
		DBGINFO("mydaemon succeed!");
	}

	FILE *fp = fopen(FNAME, "w");
	if(NULL == fp){
		DBGERR("fopen error:%s",strerror(errno));
		exit(1);
	}

	for(int i=0;;i++)
	{
		fprintf(fp,"%d\n",i);
		fflush(fp);
		//syslog(LOG_DEBUG,"%d", i);
		sleep(1);
	}

	fclose(fp);
	closelog();
	exit(0);

}
