#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	int c;
	printf("This is First Statement\n");

	c = fork();
	if(c>0)
	{
		printf("Inside Parent \n");
	}
	else
	{
		printf("Inside Child\n");
		execvp("./p1",NULL);
	}
}