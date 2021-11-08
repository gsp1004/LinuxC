#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void usage(void)
{
	printf("press 'ctrl+C' all the time and you will see the program not run 10s\n");
	return;
}


int main()
{
	alarm(10);
	alarm(1);
	alarm(5);


	while(1)
		pause();

	return 0;
}
