#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/mman.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void usage(int argc, char * argv[])
{
	printf("Program function: count how many [char] in [file]. \nUse mmap to map the [file] to var addr for better opt\n");
	printf("usage:%s [file] [char]\n", argv[0]);
	printf("   eg:%s /etc/init.d/udev a\n", argv[0]);
	return;
}

int main(int argc, char * argv[])
{
	int fd;
	struct stat statres;
	char *str;
	char target;
	if(argc<3){
		usage(argc, argv);
		exit(1);
	}

	fd = open(argv[1], O_RDONLY);
	if(fd < 0){
		perror("open()");
		exit(1);
	}

	target = argv[2][0];

	if(fstat(fd, &statres) < 0){
		perror("fstat()");
		exit(1);
	}

	str = mmap(NULL, statres.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(MAP_FAILED == str){
		perror("mmap()");
		exit(1);
	}

	close(fd);
	int count = 0;
	for(int i=0;i<statres.st_size;i++){
		if(str[i] == target)
			count++;
	}
	
	printf("There are (%d) '%c' in (%s)\n", count, target, argv[1]);
	
	munmap(str, statres.st_size);
	exit(0);
}

