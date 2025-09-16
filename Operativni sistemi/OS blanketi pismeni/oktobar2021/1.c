#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<ctype.h>

#define MAX_RECI 100
#define MAX_DUZINA 80

pthread_mutex_t mutex;
pthread_cond_t kondiciona;
int sortirano=0;
char* reci[MAX_RECI]; //niz reci
int brojac=0; //broji reci u recenici

void* sortiraj()
{
	int i, j;
	pthread_mutex_lock(&mutex);
	for(i=0;i<brojac-1;i++)
		for(j=i+1;j<brojac;j++)
		{
			if(strcmp(reci[i], reci[j])>0)
			{
				char* temp=reci[i];
				reci[i]=reci[j];
				reci[j]=temp;
			}
		}
	sortirano=1;
	pthread_cond_signal(&kondiciona);
	pthread_mutex_unlock(&mutex);
}

void* modstamp()
{
	int i;
	pthread_mutex_lock(&mutex);
	while(!sortirano)
		pthread_cond_wait(&kondiciona, &mutex);
	if(brojac>0)
	{
		reci[0][0]=toupper(reci[0][0]);
		for(i=0;i<brojac;i++)
		{
			if(i!=0)
				printf(" ");
			printf("%s", reci[i]);
		}
		printf(".\n");
	}
	pthread_mutex_unlock(&mutex);
}

void main()
{
	char recenica[80];
	int i;
	pthread_t sortirajReci, modifikujStampaj;
	
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&kondiciona, NULL);
	
	printf("Unesite recenicu: ");
	fgets(recenica, MAX_DUZINA, stdin);
	recenica[strcspn(recenica, "\n")]='\0';
	
	//Delimo recenicu na reci
	char* token = strtok(recenica, " ");
	while(token!=NULL)
	{
		reci[brojac]=strdup(token);
		brojac++;
		token=strtok(NULL, " ");
	}
	
	pthread_create(&sortirajReci, NULL, sortiraj, NULL);
	pthread_create(&modifikujStampaj, NULL, modstamp, NULL);
	
	pthread_join(sortirajReci, NULL);
	pthread_join(modifikujStampaj, NULL);
	
	printf("KRAJ\n");
	
	for(i=0;i<brojac;i++)
		free(reci[i]);
		
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&kondiciona);	
}
