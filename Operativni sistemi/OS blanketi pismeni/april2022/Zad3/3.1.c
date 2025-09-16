#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<stdio.h>
#include<unistd.h>

#define RED_PORUKA 10001
#define MAX_PORUKA 10

struct poruka
{
	long tip;
	char tekst[MAX_PORUKA];
};

int main()
{
	int pid, redid, broj=0;
	struct poruka bafer;

	pid=fork();

	if(pid<0)
	{
		printf("Greska prilikom kreiranja novog procesa\n");
		return -1;
	}

	if(pid==0)
	{
		if(execl("3", "3", NULL)<0)
			printf("Doslo je do greske prilikom ucitavanja izvrsne datoteke\n");
		exit(1);
	}
	
	redid=msgget(RED_PORUKA, IPC_CREAT | 0666);
	if(redid<0)
	{
		printf("Doslo je do greske prilikom kreiranja read\n");
		return -1;
	}
	
	do
	{
		if(msgrcv(redid, &bafer, MAX_PORUKA, 0, 0)<-1)
		{
			printf("Doslo je do greske prilikom prijema poruke\n");
			return -1;
		}
		broj=atoi(bafer.tekst);
		printf("%d\n", broj);
	}while(broj>-1);
	wait(NULL);
	msgctl(redid, IPC_RMID, 0);
	return 0;
}
