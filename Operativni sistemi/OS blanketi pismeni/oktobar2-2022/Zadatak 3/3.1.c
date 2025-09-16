#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/msg.h>

#define RED 10009
#define MAX 255

struct poruka
{
	long tip;
	char tekst[MAX];
};

int main()
{
	int redid, duzina;
	struct poruka bafer;
	char* poredjenje;
	
	redid=msgget(RED, IPC_CREAT | 0666);
	if(redid<0)
	{
		printf("Greska prilikom keiranja reda\n");
		return -1;
	}
	while(1)
	{
		//printf("USO\n");
		msgrcv(redid, &bafer, sizeof(bafer.tekst), 0, 0);
		if(strcmp(bafer.tekst, "#")==0) break;
		duzina=strlen(bafer.tekst);
		if(bafer.tip==1)
		{
			printf("\nDrugi proces je primio: %s, duzine %d\n", bafer.tekst, duzina);
			fflush(stdout);
		}
			
	}
	return 0;
}
