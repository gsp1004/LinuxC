#include<stdio.h>

#define DBGLINE(str,args...)   fprintf(stderr, "error in (%s %s %d):" str "\n",__FILE__, __FUNCTION__, __LINE__, ##args)

// error code define
#define PARAM_ERROR (-1)
#define MALLOC_ERROR (-2)
//#define _ERROR (-)

