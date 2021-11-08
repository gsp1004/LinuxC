#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>

#include"mytbf.h"


/* basic mycat
 * function: cat a file, put to stdout
 * usage: ./mycat filename
 * 
 * 
 */

#define CPS    (10)
#define BURST  (1000)
#define BUFSIZE (100)

void usage(int argc, char *argv[])
{
	fprintf(stderr, "Usage: %s filename\n"
		"\teg: %s /etc/services\n",
		argv[0], argv[0]);
	return;
}

int main(int argc, char *argv[])
{
	int sfd, dfd;
	int len = 0;
	int size, ret, pos;
	char buf[BUFSIZE];
	mytbf_t *tbf;

	if(argc < 2){
		usage();
		exit(1);
	}

	tbf = mytbf_init(CPS, BURST);
	if(NULL == tbf){
		fprintf(stderr, "mytbf_init failed!\n");
		exit(1);
	}
	
	do{
		sfd = open(argv[1], O_RDONLY);
		if(sfd < 0){
			if(errno != EINTR){
				perror("open():");
				exit(1);
			}else{
				continue;
			}
		}
	}while(sfd >= 0);

	dfd = 1;

	while(1){
		size = mytbf_fetchtoken(tbf, BUFSIZE);
		if(size < 0){
			fprintf(stderr, "mytbf_fetchtoken():%s\n", strerror(-size));
			exit(1);
		}

		while((len = read(sfd, buf, size)) < 0){
			if(errno == EINTR){
				continue;
			}
			perror("read():");
			break;
		}

		if(0 == len){
			break;
		}

		pos = 0;
		while(len > 0){
			ret = write(dfd, buf+pos,len);
			if(ret < 0){
				if(errno == EINTR){
					continue;
				}
				perror("write()");
				exit(1);
			}
			pos += ret;
			len -= ret;
		}
	}
	
	return 0;
}
