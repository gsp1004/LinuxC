#include<stdio.h>
#include<stdlib.h>
#include<glob.h>
#include <unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<pwd.h>

#include <sys/stat.h>

#if 0
struct stat {
	dev_t	  st_dev;		  /* ID of device containing file */
	ino_t	  st_ino;		  /* inode number */
	mode_t	  st_mode;		  /* protection */
	nlink_t   st_nlink; 	  /* number of hard links */
	uid_t	  st_uid;		  /* user ID of owner */
	gid_t	  st_gid;		  /* group ID of owner */
	dev_t	  st_rdev;		  /* device ID (if special file) */
	off_t	  st_size;		  /* total size, in bytes */
	blksize_t st_blksize;	  /* blocksize for filesystem I/O */
	blkcnt_t  st_blocks;	  /* number of 512B blocks allocated */

	/* Since Linux 2.6, the kernel supports nanosecond
	   precision for the following timestamp fields.
	   For the details before Linux 2.6, see NOTES. */

	struct timespec st_atim;  /* time of last access */
	struct timespec st_mtim;  /* time of last modification */
	struct timespec st_ctim;  /* time of last status change */

#define st_atime st_atim.tv_sec 	 /* Backward compatibility */
#define st_mtime st_mtim.tv_sec
#define st_ctime st_ctim.tv_sec
};

#endif


void attention(int argc, char **argv)
{
	printf("This utility's owner must be root.\n"
		"And you need to run 'sudo chmod u+s' to add s mode\n\n");
	int flag = 0;
	struct stat s;
	stat(argv[0], &s);
	if(s.st_uid != 0){
		printf("Error:owner id is %d, not root\n", s.st_uid);
		flag = 1;
	}

	if(!(s.st_mode & S_ISUID)){
		printf("Error:not set UID\n");
		flag = 1;
	}

	if(flag){
		printf("There are some error, so the utility can not run normally,exit!\n");
		exit(1);
	}

	return;
}

int main(int argc, char **argv)
{
	if(argc < 3){
		fprintf(stderr, "Usage: %s UID cmd...\n",argv[0]);
		fprintf(stderr, "   eg: %s 0 cat /etc/shadow\n",argv[0]);
		exit(1);
	}
	attention(argc, argv);

	fflush(NULL); // fflush before fork for safety
	pid_t pid = fork();
	if(pid < 0)
		exit(1);

	if(0 == pid){
		fflush(NULL);  // fflush before execXX for safety
		setuid(atoi(argv[1]));
		execvp(argv[2], argv+2);
		perror("execvp()");
		exit(1);
	}
	else{
		wait(NULL);
	}
	
	exit(0);
}
