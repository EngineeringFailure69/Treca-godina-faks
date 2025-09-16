#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
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

int main(int argc, char* argv[])
{
	int redid, pid1, pid2;
	struct poruka bafer;
	
	redid=msgget(RED, IPC_CREAT | 0666);
	if(redid<0)
	{
		printf("Greska prilikom keiranja reda\n");
		return -1;
	}
	pid1=fork();
	if(pid1==0)
	{
		if(execl("./3.1", "3.1", NULL)<0)
			printf("Greska prilikom ucitavanja prve izvrsne datoteke\n");
		return -1;
	}
	pid2=fork();
	if(pid2==0)
	{
		if(execl("./3.2", "3.2", NULL)<0)
				printf("Greska prilikom ucitavanja druge izvrsne datoteke\n");
			return -1;
	}
	do
	{
		char string[MAX];
		int cifra;
		//printf("Unesite cifru i string: ");
		scanf("%d %[^\n]", &cifra, string);
		//printf("Unesite string: ");
		//scanf("%[^\n]", string);
		//fgets(string, MAX, stdin);
		
		if(cifra!=1 && cifra!=2)
		{	
			strcpy(bafer.tekst, "#");
			bafer.tip=1;
			msgsnd(redid, &bafer, sizeof(bafer.tekst), 0);
			bafer.tip=2;
			msgsnd(redid, &bafer, sizeof(bafer), 0);
			printf("Cifra mora da bude 1 ili 2\n");
			break;
			//return -1;
		}
		bafer.tip=cifra;
		strcpy(bafer.tekst, string);	
		if(msgsnd(redid, &bafer, sizeof(bafer.tekst), 0)<-1)
		{
			printf("Greska prilikom slanja poruke\n");
			return -1;
		}
	}while(1);
	wait(NULL);
	wait(NULL);
	msgctl(redid, IPC_RMID, NULL);
	return 0;
}
