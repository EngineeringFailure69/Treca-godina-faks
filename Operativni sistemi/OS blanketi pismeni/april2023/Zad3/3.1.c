#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>

#define RED_PORUKA 10003
#define MAX_PORUKA 10
 
struct poruka
{
	long tip;
	char poruka[MAX_PORUKA];
};

int main()
{
	int pid, redid, brojac=0, broj;
	struct poruka bafer;
	char ascii;
	
	pid=fork();
	if(pid<0)
	{
		printf("Greska prilikom kreiranja novog procesa\n");
		return -1;
	}

	if(pid==0)
	{
		if(execl("3", "3", NULL)<0)
			printf("Greska prilikom otvaranja izvrsne datoteke\n");
		return -1;
	}
	
	redid=msgget(RED_PORUKA, IPC_CREAT | 0666);
	if(redid<0)
	{
		printf("Greska prilikom kreiranja reda\n");
		return -1;
	}
	
	do
	{
		if(msgrcv(redid, &bafer, MAX_PORUKA, 0, 0)<-1)
		{
			printf("Greska prilikom prijema poruke\n");
			return -1;
		}
		broj=atoi(bafer.poruka);
		ascii=broj+'0';
		printf("ASCII vrednost primljenog broja %d je %c\n", broj, ascii);
		brojac++;
	}while(broj!=0);
	printf("Primljeno je: %d brojeva", brojac);
	wait(NULL);
	msgctl(redid, IPC_RMID, 0);
	return 0;
}
