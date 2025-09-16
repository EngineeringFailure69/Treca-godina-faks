#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

int main(int argc, char* argv[])
{
	int pd1[2], pd2[2], pid;
	if(pipe(pd1)<0 || pipe(pd2)<0)
	{
		printf("Greska prilikom kreiranja jednog od dva datavoda\n");
		return -1;
	}

	if(pid=fork()==0)
	{
		char primljen[80];
		close(pd1[1]);
		close(pd2[0]);
		read(pd1[0], primljen, 80);
		printf("Proces dete je primio recenicu: %s\n", primljen);
		primljen[0]=toupper(primljen[0]);
		write(pd2[1], primljen, 80);
		close(pd1[0]);
		close(pd2[1]);
	}
	else
	{
		char primljen[80];
		close(pd1[0]);
		close(pd2[1]);
		write(pd1[1], argv[1], sizeof(char)*(strlen(argv[1])+1));
		read(pd2[0], primljen, 80);
		if(strcmp(primljen, argv[1])==0)
		{
			printf("Proces roditelj je poslao rec: %s, a od procesa deteta je primio rec: %s, reci su iste\n", argv[1], primljen);
		}
		else
		{
			printf("Proces roditelj je poslao rec: %s, a od procesa deteta je primio rec: %s, reci su razlicite\n", argv[1], primljen);
		}
		close(pd1[1]);
		close(pd2[0]);
	}
}
