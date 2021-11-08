#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

#include<unistd.h>

/* basic mycat
 * function: cat a file, put to stdout
 * usage: ./mycat filename
 * 
 * 
 */

void usage(int argc, char *argv[])
{
	fprintf(stderr, "Usage: %s filename\n"
		"\teg: %s /etc/services\n",
		argv[0], argv[0]);
	return;
}

int main(int argc, char *argv[])
{
	FILE *sfp, *dfp;
	int len = 0;
	
	if(argc < 2){
		usage();
		exit(1);
	}

	sfp = fopen(argv[1], "r");
	if(NULL == sfp){
		perror("fopen():");
		exit(1);
	}

	dfp = stdout;

	while(fread() )
	
	return 0;
}
