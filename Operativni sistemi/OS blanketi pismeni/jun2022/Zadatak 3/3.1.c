#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<unistd.h>
#include<sys/wait.h>

#define RED 10005
#define MAX 10

struct poruka
{
	long tip;
	char tekst[MAX];
};

int main()
{
	int pid, redid, i, suma=0, broj, cifra;
	struct poruka bafer;
	
	pid=fork();
	
	if(pid<0)
	{
		printf("Greska prilikom kreiranja procesa");
		return -1;
	}
	
	if(pid==0)
	{
		if(execl("3", "3", NULL)<0)
			printf("Greska prilikom ucitavanja izvrsne datoteke\n");
		return -1;
	}
	
	redid=msgget(RED, IPC_CREAT | 0666);
	if(redid<0)
	{
		printf("Doslo je do greske prilikom kreiranja reda\n");
		return -1;
	}
	for(i=0;i<10;i++)
	{
		if(msgrcv(redid, &bafer, MAX, 0, 0)<0)
		{
			printf("Doslo je do greske prilikom prijema poruke\n");
			return -1;
		}
		broj=atoi(bafer.tekst);
		printf("Primljeni broj: %d\n", broj);
		while(broj!=0)
		{
			cifra=broj%10;
			suma+=cifra;
			broj/=10;
			printf("Cifra broja je: %d\n", cifra);
		}
		printf("Suma cifara broja je: %d\n", suma);
		suma=0;
	}
	wait(NULL);
	msgctl(redid, IPC_RMID, 0);
	return 0;
}
