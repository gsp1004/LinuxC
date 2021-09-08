/* practice function related to time. 
 * functions: time, localtime, 
 * 
 */
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include<errno.h>
#include<time.h>
#include <signal.h>
#if 0 // debug
#define DBG(...) printf(__VA_ARGS__)
#else
#define DBG(...) 
#endif


#if 0
struct tm {
	int tm_sec;    /* Seconds (0-60) */
	int tm_min;    /* Minutes (0-59) */
	int tm_hour;   /* Hours (0-23) */
	int tm_mday;   /* Day of the month (1-31) */
	int tm_mon;    /* Month (0-11) */
	int tm_year;   /* Year - 1900 */
	int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
	int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
	int tm_isdst;  /* Daylight saving time */
};
#endif

#define BUFSIZE (1024)
#define FNAME "/tmp/out"

int loopflag = 1;
void endloop(int s)
{
	printf("get singal %d\n",s);
	loopflag = 0;
}

int main()
{
	char buf[BUFSIZE];
	time_t stamp;
	struct tm *t;
	FILE *fp;

	signal(SIGINT, endloop);

	fp = fopen(FNAME, "a+");
	if(!fp){
		exit(1);
	}

	int count=0;
	while(fgets(buf,BUFSIZE,fp) !=NULL){
		++count;
	}


	while(loopflag){
		stamp = time(NULL);
		//t = localtime(&stamp);
		printf("%s\n",ctime(&stamp));
		t = gmtime(&stamp);
		//strftime(buf, BUFSIZE, "%Y-%m-%d",t);
		fprintf(fp,"%-4d\t%d-%d-%d %d:%d:%d\n", ++count,
				1900 + t->tm_year,1 + t->tm_mon,t->tm_mday,
				t->tm_hour,t->tm_min,t->tm_sec
				);
		fflush(fp);
		sleep(1);
	}

	fclose(fp);
	exit(0);
}
