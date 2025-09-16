#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/wait.h>
#include<unistd.h>

#define RED 10005
#define MAX 10

struct poruka
{
	long tip;
	char tekst[MAX];
};

int main()
{
	int broj, i, redid;
	struct poruka bafer;
	
	redid=msgget(RED, IPC_CREAT | 0666);
	if(redid<0)
	{
		printf("Greska prilikom kreiranja read poruka\n");
		return -1;
	}
	
	for(i=0;i<10;i++)
	{
		printf("Unesite broj koji obradjujem: ");
		scanf("%d", &broj);
		sprintf(bafer.tekst, "%d", broj);
		
		if(msgsnd(redid, &bafer, sizeof(bafer.tekst),0)<-1)
		{
			printf("Greska prilikom slanja poruke\n");
			return -1;
		}	
	}
	return 0;
}
