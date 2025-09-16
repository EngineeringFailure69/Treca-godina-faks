#include<stdio.h>
#include<pthread.h>
#include<stdbool.h>

bool brojUnesen = false;
pthread_mutex_t mutex;
pthread_cond_t kondiciona;
pthread_t drugaNit;
int n; 
int i;

void* broji(void* arg)
{
	pthread_mutex_lock(&mutex);
	while(brojUnesen == false)
	{
		pthread_cond_wait(&kondiciona, &mutex);
	}
	int broj = n;
	int i;
	printf("Nit broji: ");
	for(i=0;i<=broj;i++)
	{
		printf("%d ", i);
		fflush(stdout);
		sleep(1);
	}
	printf("\n");
	pthread_mutex_unlock(&mutex);
}

void main()
{
	bool f=false;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&kondiciona, NULL);
	while(!f)
	{
		pthread_create(&drugaNit, NULL, broji, (void* )&n);
		pthread_mutex_lock(&mutex);
		printf("Unesite broj: ");
		scanf("%d", &n);
		if(n!=99)
		{
			brojUnesen=true;
			pthread_cond_signal(&kondiciona);
			pthread_mutex_unlock(&mutex);
			pthread_join(drugaNit, NULL);
			brojUnesen=false;
		}
		else
		{
			f=true;
		}
	}
	printf("Uneli ste 99, program prestaje da se izvrsava\n");
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&kondiciona);
}
