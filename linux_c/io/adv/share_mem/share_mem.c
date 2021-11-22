#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/mman.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>

/*
 use mmap to create a share memery to realize parent and child IPC
child : write 'Hello!'
parent: read string from child
 */
#define MEMSIZE (1024)

int main(int argc, char * argv[])
{
	char *ptr;
	//void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
	ptr = mmap(NULL, MEMSIZE, PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(ptr == MAP_FAILED){
		perror("mmap()");
		exit(1);
	}
	int pid = fork();
	if(pid < 0){
		perror("fork()");
		munmap(ptr, MEMSIZE);
		exit(0);
	}

	if(pid == 0){  // child write
		strcpy(ptr, "Hello!");
		puts("Child process write 'Hello!' finished");
		munmap(ptr, MEMSIZE);
		exit(0);
	}else{  // parent read
		wait(NULL);
		puts("Parent process wait finished");
		puts(ptr);
		munmap(ptr, MEMSIZE);
		exit(0);
	}

	exit(0);
}

