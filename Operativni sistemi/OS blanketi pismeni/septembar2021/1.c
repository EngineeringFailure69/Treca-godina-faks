#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

void* broji(void* arg)
{
	int granica=*((int* )arg);
	int i;
	printf("Nit broji: ");
	for(i=0;i<=granica;i++)
	{
		printf("%d ", i);
		fflush(stdout);
		sleep(3);
	}
	printf("\n");
} 

void main()
{
	pthread_t nit;
	char broj[10];
	int granica;
	do
	{
		printf("Unesite broj do kojeg brojim, ili KRAJ za zavrsetak programa: ");
		fgets(broj, 10, stdin);
		broj[strcspn(broj, "\n")]='\0';
		if(strcmp(broj, "KRAJ")==0)
		{
			printf("Uneli ste KRAJ, program se zavrsava\n");
			break;
		}
		granica=atoi(broj);
		pthread_create(&nit, NULL, broji, (void* )&granica);
		pthread_join(nit, NULL);
	}while(strcmp(broj, "KRAJ")!=0);
}
