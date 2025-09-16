#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>

int bafer[10];

void* generisi()
{
	int i, broj;
	for(i=0;i<10;i++)
	{
		broj=rand()%199;
		bafer[i]=broj;
		printf("Nit je na poziciju %d upisala broj %d\n", i, bafer[i]);
		fflush(stdout);
		sleep(1);
	}
}

void main()
{
	srand(time(NULL));
	pthread_t nit;
	int suma=0, i;
	while(suma<=1000)
	{
		suma=0;
		pthread_create(&nit, NULL, generisi, NULL);
		pthread_join(nit, NULL);
		for(i=0;i<10;i++)
		{
			suma+=bafer[i];
		}	
		printf("Suma je: %d\n", suma);
		if(suma>800)
		{
			printf("Suma brojeva je veca od 800\n");
		}
		else if(suma<800)
		{
			printf("Suma brojeva je manja od 800\n");
		}
		else
		{
			printf("Suma brojeva je 800\n");
		}
		printf("Niz je: ");
		for(i=0;i<10;i++)
		{
			printf("%d ", bafer[i]);
		}
		printf("\n\n");
	}
	printf("Suma brojeva je veca od 1000, program se zavrsava\n");
}
