#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include<errno.h>

#if 0 // debug
#define DBG(...) printf(__VA_ARGS__)
#else
#define DBG(...) 
#endif


int main(int argc, char *argv[])
{
	//int getopt(int argc, char * const argv[], const char *optstring);
	int c;
	while(1)
	{
		c = getopt(argc, argv, "a:bcde"); // a with ':' means has argument behind a(xx -a 3)
		if(c<0){
			break;
		}

		switch(c){
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
				printf("opt %c with argument %s\n", c, optarg);
				break;
			default:
				break;

		}
	}
}
