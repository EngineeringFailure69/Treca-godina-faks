#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>

#define MAX_LINE 80

int main()
{
	int pd[2], pid;
	char filename[MAX_LINE];
	printf("Unesite ime datoteke: ");
	scanf("%s", &filename);

	if(pipe(pd)==-1)
	{
		printf("Greska prilikom kreiranja datavoda!\n");
		exit(-1);
	}

	if(pid=fork()>0)
	{
		FILE* f;
		char buff[MAX_LINE];
		int broj=0;

		close(pd[0]);
		f=fopen(filename, "r");
		while(!feof(f))
		{
			strcpy(buff, "");
			fgets(buff, MAX_LINE, f);
			if(strstr(buff, "PROCES"))
				broj++;
		}

		write(pd[1], &broj, sizeof(int));
		fclose(f);
		close(pd[1]);
		wait(NULL);
		exit(0);
	}
	else
	{
		int broj=0;
		close(pd[1]);
		read(pd[0], &broj, sizeof(int));
		printf("Broj pojavljivanja reci PROCES je: %d\n", broj);
		close(pd[0]);
		exit(0);
	}
}

