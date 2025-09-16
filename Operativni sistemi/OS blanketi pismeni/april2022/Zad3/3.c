#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<time.h>

#define RED_PORUKA 10001
#define MAX_PORUKA 10

struct poruka
{
	long tip;
	char tekst[MAX_PORUKA];
};

int main()
{
	int N, redid, i;
	struct poruka bafer;
	srand(time(NULL));
	
	redid=msgget(RED_PORUKA, IPC_CREAT | 0666);
	if(redid<0)
	{
		printf("Doslo je do greske prilikom kreiranja reda\n");
		return -1;
	}
	
	N=rand()%20;
	for(i=0;i<N+1;i++)
	{
		if(i==N)
			sprintf(bafer.tekst, "%d", -1);
		else
		{
			sprintf(bafer.tekst, "%d", rand()%100);
		}
		bafer.tip=1;
		if(msgsnd(redid, &bafer, sizeof(bafer.tekst), 0)<-1)
		{
			printf("Greska prilikom slanja poruke\n");
			return -1;
		}
	}
	return 0;
}
