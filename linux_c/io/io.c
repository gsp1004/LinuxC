#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
	putchar('a');  // putchar() in stdio.h
	write(1, "b", 1);  // write() in unistd.h

	putchar('a');
	write(1, "b", 1);

	putchar('a');
	write(1, "b", 1);
}
