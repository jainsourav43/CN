#include <stdio.h>
#include <semaphore.h>
#include<pthread.h>
sem_t s1,s2;
int x,y;
void *updatex(void* p)
{
	while(1)
	{
		sem_wait(&s2);
		x=y+1;
		printf("X=  %d\n",x);
		sleep(1);
		sem_post(&s1);
	}
}
void *updatey(void* p)
{
	while(1)
	{
		sem_wait(&s1);
		y=x+1;
		printf("Y=  %d\n",y);
		sleep(1);
		sem_post(&s2);
	}
}


int main(int argc, char const *argv[])
{
	sem_init(&s1,0,0);
	sem_init(&s2,0,1);
	pthread_t t1,t2;
	pthread_create(&t1,NULL,*updatex,NULL);
	pthread_create(&t2,NULL,*updatey,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);




	return 0;
}