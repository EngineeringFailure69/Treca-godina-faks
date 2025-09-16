#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include<sys/wait.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<sys/types.h>

#define RED 10009
#define MAX 255

struct poruka
{
	long tip;
	char tekst[MAX];
};

int main()
{
	int redid, i;
	struct poruka bafer;
	
	redid=msgget(RED, IPC_CREAT | 0666);
	if(redid<0)
	{
		printf("Greska prilikom keiranja reda\n");
		return -1;
	}
	
	while(1)
	{
		msgrcv(redid, &bafer, sizeof(bafer.tekst), 2, 0);
		if(strcmp(bafer.tekst, "#")==0) break;
		if(bafer.tip==2)
		{
			//int i;
		for(i=0;bafer.tekst[i];i++)
			bafer.tekst[i]=toupper(bafer.tekst[i]);
			//uVelika();
			printf("Treci proces je primio: %s\n", bafer.tekst);
			fflush(stdout);
		}
	}
}
