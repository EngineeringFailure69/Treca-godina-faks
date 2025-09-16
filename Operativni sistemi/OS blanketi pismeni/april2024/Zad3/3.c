#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<sys/wait.h>

#define RED 10010
#define MAX 100

struct poruka
{
	long tip;
	char tekst[MAX];
};

int sum_cifre(int broj)
{
	int sum=0;
	while(broj>0)
	{
		sum+=broj%10;
		broj/=10;
	}
	return sum;
}

int main()
{
	int redid, pid1, pid2;
	struct poruka bafer;
	
	redid=msgget(RED, IPC_CREAT | 0666);
	if(redid<0)
	{
		printf("Doslo je do greske prilikom kreiranja reda\n");
		return -1;
	}
	
	if(pid1=fork()==0)
	{
		FILE* fajl=fopen("neparni", "w");
		if(fajl==NULL)
		{
			printf("Ne mogu da otvorim fajl neparni.txt\n");
			exit(1);
		}
		while(1)
		{
			msgrcv(redid, &bafer, sizeof(bafer.tekst), 1, 0);
			int broj = atoi(bafer.tekst);
			if(broj==0) break;
			int sum = sum_cifre(broj);
			fprintf(fajl, "Broj: %d, suma cifara: %d\n", broj, sum);
			fflush(fajl);
		}
		fclose(fajl);
		exit(0);
	}

	if(pid2=fork()==0)
	{
		FILE* fajl=fopen("parni", "w");
		if(fajl==NULL)
		{
			printf("Ne mogu da otvorim fajl parni.txt\n");
			exit(1);
		}
		while(1)
		{
			msgrcv(redid, &bafer, sizeof(bafer.tekst), 2, 0);
			int broj=atoi(bafer.tekst);
			if(broj==0) break;
			int sum=sum_cifre(broj);
			fprintf(fajl, "Broj: %d, suma cifara: %d\n", broj, sum);
			fflush(fajl);
		}
		fclose(fajl);
		exit(0);
	}

	while(1)
	{
		int broj;
		printf("Unesite broj ili 0 za kraj: ");
		scanf("%d", &broj);
		
		bafer.tip = (broj%2==0)? 2:1;
		sprintf(bafer.tekst, "%d", broj);
		msgsnd(redid, &bafer, sizeof(bafer.tekst), 0);
		
		if(broj==0) break;
	}
	printf("Uneli ste 0, program se zavrsava\n");
	wait(NULL);
	wait(NULL);
	msgctl(redid, IPC_RMID, NULL);
	
	return 0;
}
