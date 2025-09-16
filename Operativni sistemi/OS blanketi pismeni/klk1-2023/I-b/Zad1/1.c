#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<stdbool.h>
#include<string.h>
#include<unistd.h>

bool brojUnesen=false;
pthread_mutex_t mutex;
pthread_cond_t kondiciona;
pthread_t nit;
int n;

void* ucitajIzFajla()
{
	int i, count=0;
	FILE* fajl;
	char linija[80];
	pthread_mutex_lock(&mutex);
	while(brojUnesen==false)
	{
		pthread_cond_wait(&kondiciona, &mutex);
	}
	fajl=fopen("data", "r");
	if(fajl==NULL)
	{
		printf("Ne mogu da otvorim fajl!\n");
		exit(EXIT_FAILURE);
	}
	while(fgets(linija, sizeof(linija), fajl)!=NULL && count < n)
	{
		printf("Iz fajla je procitana linija: %s\n", linija);
		count++;
		sleep(1);
	}
	printf("\n");
	fclose(fajl);
	count=0;
	pthread_mutex_unlock(&mutex);
}

void main()
{
	bool f=false;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&kondiciona, NULL);
	while(!f)
	{
		pthread_create(&nit, NULL, ucitajIzFajla, NULL);
		pthread_mutex_lock(&mutex);
		printf("Unesite broj linija koje citate iz fajla: ");
		scanf("%d", &n);
		if(n!=99)
		{
			brojUnesen=true;
			pthread_cond_signal(&kondiciona);
			pthread_mutex_unlock(&mutex);
			pthread_join(nit, NULL);
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
