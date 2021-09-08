#define _XOPEN_SOURCE       /* See feature_test_macros(7) */
//#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include<errno.h>
#include<pwd.h>
#include <shadow.h>
#include <crypt.h>


#if 0 // debug
#define DBG(...) printf(__VA_ARGS__)
#else
#define DBG(...) 
#endif

#if 0

struct spwd {
	char *sp_namp;     /* Login name */
	char *sp_pwdp;     /* Encrypted password */
	long  sp_lstchg;   /* Date of last change
						  (measured in days since
						  1970-01-01 00:00:00 +0000 (UTC)) */
	long  sp_min;      /* Min # of days between changes */
	long  sp_max;      /* Max # of days between changes */
	long  sp_warn;     /* # of days before password expires
						  to warn user to change it */
	long  sp_inact;    /* # of days after password expires
						  until account is disabled */
	long  sp_expire;   /* Date when account expires
						  (measured in days since
						  1970-01-01 00:00:00 +0000 (UTC)) */
	unsigned long sp_flag;  /* Reserved */
};
#endif

int main(int argc, char *argv[])
{
	if(argc<2)
	{
		fprintf(stderr,"Usage...with username\n");
		exit(1);
	}

	char *pass = getpass("enter your passwd:");
	struct spwd *shadowline = getspnam(argv[1]);
	if(NULL == shadowline){
		perror("getspnam:");
		exit(1);
	}

	char * crypted_pass = crypt(pass, shadowline->sp_pwdp);

	if(strcmp(shadowline->sp_pwdp, crypted_pass) == 0)
		puts("ok!");
	else
		puts("failed!");

	exit(0);
}
