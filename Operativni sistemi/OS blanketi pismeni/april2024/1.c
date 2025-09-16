#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LENGTH 100

struct podaci
{
	int indeks;
	char rec[MAX_WORD_LENGTH];
	int brSamoglasnika;
};

void* broji_samoglasnike(void* arg)
{
 	struct podaci *podaci = (struct podaci* )arg;
	char* rec=podaci->rec;
	int brojac=0, i;
	for(i=0;rec[i]!='\0';i++)
	{
		char ch=tolower(rec[i]);
		if(ch=='a' || ch=='e' || ch=='i' || ch=='o' || ch=='u')
			brojac++;
	}
	podaci->brSamoglasnika=brojac;
}

void main()
{
	char recenica[1024];
	char* reci[MAX_WORDS];
	char* token;
	int broj_reci=0, i;
	struct podaci pod[MAX_WORDS];
	pthread_t niti[MAX_WORDS];
	printf("Unesite recenicu: ");
	fgets(recenica, sizeof(recenica), stdin);
	recenica[strcspn(recenica, "\n")]='\0';
	
	token=strtok(recenica, " ");
	while(token!=NULL && broj_reci<MAX_WORDS)
	{
		reci[broj_reci++]=token;
		token=strtok(NULL, " ");
	}
	
	for(i=0;i<broj_reci;i++)
	{
		pod[i].indeks=i+1;
		strcpy(pod[i].rec, reci[i]);
		pod[i].brSamoglasnika=0;
		pthread_create(&niti[i], NULL, broji_samoglasnike, (void* )&pod[i]);
	}
	
	for(i=0;i<broj_reci;i++)
		pthread_join(niti[i], NULL);
		
	for(i=0;i<broj_reci;i++)
	{
		printf("Rec broj %d: %s, broj samoglasnika: %d\n", pod[i].indeks, pod[i].rec, pod[i].brSamoglasnika);
	}	
}
