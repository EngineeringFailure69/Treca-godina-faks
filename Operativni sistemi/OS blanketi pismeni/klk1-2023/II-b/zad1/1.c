#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<semaphore.h>

#define MAX_LEN 80
char linija[MAX_LEN];
int linijaSpremna=0;
int turn=1;
sem_t semparni, semneparni, mutex;//, turn;

void* neparnaLinija()
{
	FILE* neparni=fopen("neparni.txt", "w");
	if(neparni==NULL)
	{
		printf("Ne mogu da otvorim fajl neparni.txt!\n");
		//return;
		exit(1);
	}
	while(1)
	{
		sem_wait(&semneparni);
		if(linijaSpremna==1 && (turn % 2!=0))
		{
			fprintf(neparni, "%s\n", linija);
			//fputs(linija, neparni);
			fflush(neparni);
			linijaSpremna=0;
			printf("U fajl neparni.txt je upisana linija: %s\n", linija);
		}
		sem_post(&mutex);
	}
	fclose(neparni);
	//return NULL;
}

void* parnaLinija()
{
	FILE* parni=fopen("parni.txt", "w");
	if(parni==NULL)
	{
		printf("Ne mogu da otvorim fajl parni.txt!\n");
		//return;
		exit(1);
	}
	while(1)
	{
		sem_wait(&semparni);
		if(linijaSpremna==1 /*&& (turn%2==0)*/)
		{
			fprintf(parni, "%s\n", linija);
			//fputs(linija, parni);
			fflush(parni);
			linijaSpremna=0;
			printf("U fajl parni.txt je upisana linija: %s\n", linija);
		}
		sem_post(&mutex);
	}
	fclose(parni);
	//return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t nitparni, nitneparni;
	FILE* fajl;
	
	sem_init(&semparni, 0, 0);
	sem_init(&semneparni, 0, 0);
	sem_init(&mutex, 0, 1);
	//sem_init(&turn, 0, 1);
	
	if(argc<2)
	{
		printf("Morate da unesete naziv datoteke iz koje citam!\n");
		return 1;
	}
	
	fajl=fopen(argv[1], "r");
	rewind(fajl);
	if(fajl==NULL)
	{
		printf("Ne mogu da otvorim fajl!\n");
		return 1;
	}
	
	pthread_create(&nitneparni, NULL, neparnaLinija, NULL);
	pthread_create(&nitparni, NULL, parnaLinija, NULL);
	
	while(fgets(linija, sizeof(linija), fajl))
	{
		//linijaSpremna=1;
		sem_wait(&mutex);
		printf("Procitana je linija: %s\n", linija);
		//linijaSpremna=1;
		//turn++;
		if(turn%2==0)
		{
			sem_post(&semneparni);	
			linijaSpremna=1;
			//turn=1;
		}
		else
		{	
			sem_post(&semparni);
			linijaSpremna=1;
			//turn=0;
		}
		turn++;
	}
	
	pthread_join(nitneparni, NULL);
	pthread_join(nitparni, NULL);
		
	fclose(fajl);
	sem_destroy(&semparni);
	sem_destroy(&semneparni);
	sem_destroy(&mutex);
	//sem_destroy(&turn);
	return 0;
}
