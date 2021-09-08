#include<stdio.h>
#include<stdlib.h>
#include<glob.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include<errno.h>
#include<pwd.h>

#if 0 // debug
#define DBG(...) printf(__VA_ARGS__)
#else
#define DBG(...) 
#endif

#if 0
struct passwd {
	char   *pw_name;       /* username */
	char   *pw_passwd;     /* user password */
	uid_t   pw_uid;        /* user ID */
	gid_t   pw_gid;        /* group ID */
	char   *pw_gecos;      /* user information */
	char   *pw_dir;        /* home directory */
	char   *pw_shell;      /* shell program */
};
#endif

int main(int argc, char *argv[])
{
	struct passwd *pwdline;
	if(argc < 2){
		fprintf(stderr, "Usage...\n");
		exit(1);
	}
	pwdline = getpwuid(atoi(argv[1]));
	if(NULL == pwdline){
		perror("getpwuid error:");
		exit(1);
	}

	puts(pwdline->pw_name);


}
