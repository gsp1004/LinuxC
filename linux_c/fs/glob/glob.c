#include<stdio.h>
#include<stdlib.h>
#include<glob.h>

#define PAT "/etc/a*.conf"
/*
   typedef struct {
   size_t   gl_pathc;    // Count of paths matched so far  
   char   **gl_pathv;    // List of matched pathnames.  
   size_t   gl_offs;     // Slots to reserve in gl_pathv.  
   } glob_t;
 */
int main(int argc, char *argv[])
{
	glob_t g;

	if(glob(PAT, 0, NULL, &g)){
		exit(1);
	}
	
	for(int i=0;i<g.gl_pathc;i++){
		puts(g.gl_pathv[i]);
	}

	globfree(&g);
	exit(0);
}
