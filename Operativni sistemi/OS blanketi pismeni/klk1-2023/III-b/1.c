#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>

#define MAX_LEN 80
pthread_mutex_t mutex;
pthread_cond_t kondiciona1;
pthread_cond_t kondiciona2;
char recenica[MAX_LEN];
bool stop=false;
bool spremnaPrva=false;
bool spremnaDruga=false;

void* prvaNit()
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(spremnaPrva==false)
			pthread_cond_wait(&kondiciona1, &mutex);
		if(stop)
		{
			pthread_mutex_unlock(&mutex);
			break;
		}
		recenica[0]=toupper(recenica[0]);
		if(recenica[strlen(recenica)-1]!='.')
		{
			recenica[strlen(recenica+1)]='\0';
			recenica[strlen(recenica)]='.';
		}
		printf("Recenica posle prve promene: %s\n", recenica);
		spremnaPrva=false;
		spremnaDruga=true;
		pthread_cond_signal(&kondiciona2);
		pthread_mutex_unlock(&mutex);
	}
}

void* drugaNit()
{
	int i;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(spremnaDruga==false)
			pthread_cond_wait(&kondiciona2, &mutex);
		if(stop)
		{
			pthread_mutex_unlock(&mutex);
			break;
		}
		for(i=0;i<strlen(recenica);i++)
		if(recenica[i]==' ')
			recenica[i]='_';
		printf("Finalna recenica: %s\n", recenica);
		spremnaDruga=false;
		pthread_mutex_unlock(&mutex);
	}
}

void main()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&kondiciona1, NULL);
	pthread_cond_init(&kondiciona2, NULL);
	pthread_t nit1, nit2;
	pthread_create(&nit1, NULL, prvaNit, NULL);
	pthread_create(&nit2, NULL, drugaNit, NULL);
	
	while(1)
	{
		printf("Unesite recenicu: ");
		gets(recenica);
		
		if(strcmp(recenica, "KRAJ")==0)
		{
			pthread_mutex_lock(&mutex);
			spremnaPrva=true;
			spremnaDruga=true;
			stop=true;
			pthread_cond_signal(&kondiciona1);
			pthread_cond_signal(&kondiciona2);
			pthread_mutex_unlock(&mutex);
			break;
		}
			pthread_mutex_lock(&mutex);
			spremnaPrva=true;		
			pthread_cond_signal(&kondiciona1);
			pthread_mutex_unlock(&mutex);
			
			pthread_mutex_lock(&mutex);
			while(spremnaDruga==true)
				pthread_cond_wait(&kondiciona2, &mutex);	
			pthread_mutex_unlock(&mutex);
	}
	
	printf("Uneli ste KRAJ, program prestaje da se izvrsava\n");
	
	pthread_join(nit1, NULL);
	pthread_join(nit2, NULL);
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&kondiciona1);
	pthread_cond_destroy(&kondiciona2);
}
