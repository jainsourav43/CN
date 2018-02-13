#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	int c;
	printf("This is  new P1  Statement\n");
	execvp("./p2",NULL);
}