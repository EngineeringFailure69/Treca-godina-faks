#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char* argv[])
{
	int pid;
	if(argc!=3)
	{
		printf("Greska pri koriscenju, format mora biti: <program koji pokrecemo> <argument_koji_saljemo>\n");
		return 1;
	}
	pid=fork();
	if(pid<0)
	{
		printf("Greska prilikom kreiranja dodatnog procesa\n");
		return 1;
	}
	if(pid==0)
	{
		execlp(argv[1], argv[1], argv[2], (char* )NULL);
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
		if(WIFEXITED(status))
		{
			printf("Proces dete je izasao sa kodom: %d\n", WEXITSTATUS(status));
			return 0;
		}
		else
		{
			printf("Proces dete je neuspesno izasao\n");
			return 0;
		}
	}
	
	return 0;
}
