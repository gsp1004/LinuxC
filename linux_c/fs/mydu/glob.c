#include<stdio.h>
#include<stdlib.h>
#include<glob.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#if 0 // debug
#define DBG(...) printf(__VA_ARGS__)
#else
#define DBG(...) 
#endif

/*
   typedef struct {
   size_t   gl_pathc;    // Count of paths matched so far  
   char   **gl_pathv;    // List of matched pathnames.  
   size_t   gl_offs;     // Slots to reserve in gl_pathv.  
   } glob_t;
 */

#if 0
struct stat {
	dev_t     st_dev;         /* ID of device containing file */
	ino_t     st_ino;         /* inode number */
	mode_t    st_mode;        /* protection */
	nlink_t   st_nlink;       /* number of hard links */
	uid_t     st_uid;         /* user ID of owner */
	gid_t     st_gid;         /* group ID of owner */
	dev_t     st_rdev;        /* device ID (if special file) */
	off_t     st_size;        /* total size, in bytes */
	blksize_t st_blksize;     /* blocksize for filesystem I/O */
	blkcnt_t  st_blocks;      /* number of 512B blocks allocated */

	/* Since Linux 2.6, the kernel supports nanosecond
	   precision for the following timestamp fields.
	   For the details before Linux 2.6, see NOTES. */

	struct timespec st_atim;  /* time of last access */
	struct timespec st_mtim;  /* time of last modification */
	struct timespec st_ctim;  /* time of last status change */

#define st_atime st_atim.tv_sec      /* Backward compatibility */
#define st_mtime st_mtim.tv_sec
#define st_ctime st_ctim.tv_sec
};
#endif
#define PATHSIZE (1024)



int isDotOrDotDot(const char *path)
{
	char * s = strrchr(path, '/');
	return(!strcmp(s+1,".") || !strcmp(s+1,".."));
#if 0
	if(!strcmp(s+1,".") || !strcmp(s+1,"..")){
		DBG("JUMP:%s\n",path);
		return 1;
	}else{
		DBG("GET:%s\n",path);
		return 0;
	
	}
#endif
}


/*
 * return bolcks
 */
int du(const char *path)
{
	glob_t g;
	static char nextpath[PATHSIZE] = {0};
	static struct stat s;
	int sum = 0;

	lstat(path, &s);
	if(!S_ISDIR(s.st_mode)){ // not dir
		DBG("BLOCKS = %ld\n",s.st_blocks);
		return s.st_blocks;
	}
	else
	{
		sum = s.st_blocks;

		strcpy(nextpath,path);
		strcat(nextpath,"/*");
		glob(nextpath, 0, NULL, &g);

		strcpy(nextpath,path);
		strcat(nextpath,"/.*");
		glob(nextpath, GLOB_APPEND, NULL, &g);

		for(int i=0;i<g.gl_pathc;i++){
			if(!isDotOrDotDot(g.gl_pathv[i])){
				//DBG("path = %s\n",g.gl_pathv[i]);
				sum += du(g.gl_pathv[i]);
			}
		}
	}
	return sum;
}







int main(int argc, char *argv[])
{
	char path[PATHSIZE] = {0};
	if(argc < 2)
	{
		getcwd(path, PATHSIZE);
	}
	else
	{
		strcpy(path, argv[1]);
	}

	char cmd[1024] = {0};
	sprintf(cmd,"du --block-size=512 %s",path);
	printf("%d blocks, run [%s] to test the result\n",du(path), cmd);

	FILE * fp = popen(cmd, "r");
	char *str = NULL;
	size_t size = 0;
	printf("result of cmd: %s\n",cmd);
	while(getline(&str, &size, fp) >=0){
		printf("%s",str);
	}
	return 0;
}












