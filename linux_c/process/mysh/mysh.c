#include<stdio.h>
#include<stdlib.h>
#include<glob.h>
#include <unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<pwd.h>


struct cmd_st{
	glob_t globres;
};
/*
   typedef struct {
   size_t   gl_pathc;    // Count of paths matched so far  
   char   **gl_pathv;    // List of matched pathnames.  
   size_t   gl_offs;     // Slots to reserve in gl_pathv.  
   } glob_t;
 */
static void prompt(void)
{
	char s[1024];
	printf("%s@mysh-0.1:%s$ ", getpwuid(getuid())->pw_name, getcwd(s, sizeof(s)));
}

#define DELIMS " \t\n"

// parse the string get from stdin
static int parse(char *line, struct cmd_st *res)
{
	char *tok;
	int tmp = 0;
	while(1){
		tok = strsep(&line, DELIMS);
		if(NULL == tok){  // 解析完毕，退出
			break;
		}
		
		if(tok[0] == '\0'){  // 连续分隔符处理
			continue;
		}
		
		if(glob(tok, GLOB_NOCHECK|GLOB_APPEND*tmp, NULL, &res->globres)){
			perror("glob()");
			return -1;
		}
		tmp = 1;
	}
	if(0 == tmp){  // glob 没有解析结果
		return -1;
	}

	return 0;
}

int main()
{
	char *str = NULL;
	size_t str_size = 0;
	pid_t pid;
	struct cmd_st cmd;
	
	while(1){
		prompt();
		
		if(0 > getline(&str, &str_size,stdin)){
			perror("getline()");
			exit(1);
		}

		if(parse(str, &cmd)){
			continue;
		}

		if(0) // inside
		{

		}
		else  // outside cmd
		{
			pid = fork();
			if(0 > pid){
				perror("fork()");
				exit(1);
			}
			if(0 == pid) // child
			{
				execvp(cmd.globres.gl_pathv[0], cmd.globres.gl_pathv);
				perror("execvp()");
				exit(1);
			}
			else  // parent
			{
				wait(NULL);
				globfree(& cmd.globres);
			}
		}
	}
	
	exit(0);
}
