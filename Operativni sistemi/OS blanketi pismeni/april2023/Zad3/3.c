#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<ctype.h>

#define RED_PORUKA 10003
#define MAX_PORUKA 10

struct poruka
{
	long tip;
	char poruka[MAX_PORUKA];
};

int main()
{
	int redid, n, tip;
	struct poruka bafer;
	
	redid=msgget(RED_PORUKA, IPC_CREAT | 0666);
	if(redid<0)
	{
		printf("Doslo je do greske prilikom kreiranja reda\n");
		return -1;
	}
	do
	{
		printf("Unesite broj koji saljemo, mora da bude u intervalu 0-127: ");
		scanf("%d", &n);
		if(n==0 || n<0 || n>127)
			break;
		sprintf(bafer.poruka, "%d", n);
		if(msgsnd(redid, &bafer, sizeof(bafer.poruka), 0)<-1)
		{
			printf("Greska prilikom slanja poruke\n");
			return -1;
		}
	}while(n!=0);
	return 0;
}
