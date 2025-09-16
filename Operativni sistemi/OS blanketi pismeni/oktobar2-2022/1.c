#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<unistd.h>

void* broji(void* arg)
{
	int i, granica;
	granica=*((int* )arg);
	printf("Nit broji: ");
	for(i=0;i<=granica;i++)
	{
		if(i==granica)
		{
			printf("%d ", i);
			fflush(stdout);
			sleep(1);
		}
		else
		{
			printf("%d, ", i);
			fflush(stdout);
			sleep(1);
		}
	}
	printf("\n");
}

void main()
{
	pthread_t nit;
	char br[4];
	int broj;
	printf("Unesite broj do kojeg brojim, ili unesite KRAJ za zavrsetak: ");
	scanf("%s", &br);
	while(strcmp(br, "KRAJ")!=0)
	{
		broj=atoi(br);
		pthread_create(&nit, NULL, broji, (void* )&broj);
		pthread_join(nit, NULL);
		printf("Unesite broj do kojeg brojim, ili unesite KRAJ za zavrsetak: ");
		scanf("%s", &br);
	}
	printf("Uneli ste KRAJ, program se zavrsava\n");
}
