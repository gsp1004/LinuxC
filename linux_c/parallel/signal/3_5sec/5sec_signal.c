#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<signal.h>
static volatile int loop = 1; // why use volatile, because the gcc may try to optimize the while(loop) to while(1) when we run gcc -O1

static void handler(int s)
{
	loop = 0;
}


int main()
{
	signal(SIGALRM, handler);
	alarm(5);

	long long int count = 0;
	while(loop)
		count++;
	printf("%lld\n", count);
	return 0;
}
