#include<stdio.h>
#include<pthread.h>

int niz[100];

void* sortiraj(void* arg)
{
	int n = *((int* )arg);
	int i, j, pom;
	for(i=0;i<n-1;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(niz[i]>niz[j])
			{
				pom=niz[i];
				niz[i]=niz[j];
				niz[j]=pom;
			}
		}
	}
}

void main()
{
	FILE *file;
	int n=0, i;
	pthread_t nit;
	file=fopen("ulaz.txt", "r");
	while(fscanf(file, "%d", &niz[n])!=EOF)
	{
		n++;
	}
	fclose(file);
	pthread_create(&nit, NULL, sortiraj, (void* )&n);
	pthread_join(nit, NULL);
	printf("Niz nakon sortiranja: ");
	for(i=0;i<n;i++)
	{
		printf("%d ", niz[i]);
	}
	printf("\n");
}
