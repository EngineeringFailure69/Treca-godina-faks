#include<stdio.h>
#include<pthread.h>

int bafer[10];
pthread_mutex_t mutex1, mutex2;

void* prva_nit()
{
	int i, broj;
	for(i=0;i<10;i=i+2)
	{
		broj = rand() % 299;
		bafer[i] = broj;
		printf("Nit 1 je na poziciju %d upisala broj %d\n", i, bafer[i]);
		fflush(stdout);
		sleep(1);
	}
}

void* druga_nit()
{
	int i, broj;
	for(i=1;i<10;i=i+2)
	{
		broj = 300 + rand() % 199;
		bafer[i] = broj;
		printf("Nit 2 je na poziciju %d upisala broj %d\n", i, bafer[i]);
		fflush(stdout);
		sleep(1);
	}
}

void main()
{
	pthread_t nit1, nit2;
	int i, j, suma=0;
	for(i=0;i<5;i++)
	{
		pthread_create(&nit1, NULL, prva_nit, NULL);
		pthread_create(&nit2, NULL, druga_nit, NULL);
		pthread_join(nit1, NULL);
		pthread_join(nit2, NULL);
		for(j=0;j<10;j++)
		{
			suma += bafer[j];
		}
		printf("Suma je: %d\n", suma);
		if(suma>2000)
		{
			printf("Suma brojeva je veca od 2000\n");
		}
		else if(suma<2000)
		{
			printf("Suma brojeva je manja od 2000\n");
		}
		else
		{
			printf("Suma brojeva je 2000\n");
		}
		suma=0;
		printf("Niz je: ");
		for(j=0;j<10;j++)
		{
			printf("%d ", bafer[j]);
		}
		printf("\n\n");
	}
}

