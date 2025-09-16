#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/file.h>
#include<sys/wait.h>

#define MAX 80

int main(int argc, char* argv[])
{
	int pd1[2], pd2[2], pid, broj;
	FILE* fajl;
	
	if(argc!=3)
	{
		printf("Upotreba: <putanja_do_txt> <broj> \n");
		return -1;
	}
	if(pipe(pd1)==-1)
	{
		printf("Greska prilikom otvaranja prvog datavoda\n");
		return -1;
	}
	if(pipe(pd2)==-1)
	{
		printf("Greska prilikom otvaranja drugog datavoda\n");
		return -1;
	}
	broj=atoi(argv[2]);
	pid=fork();
	if(pid>0)
	{
		int rezultat;
		close(pd1[0]);
		close(pd2[1]);
		write(pd1[1], &broj, sizeof(int));
		write(pd1[1], argv[1], strlen(argv[1])+1);
		printf("Prvi proces je poslao: %s %d\n", argv[1], broj);
		while(read(pd2[0], &rezultat, sizeof(int))>0)
			printf("Rec duza od unetog broja je na liniji: %d\n", rezultat);
		close(pd1[1]);
		close(pd2[0]);
		wait(NULL);
	}
	else
	{
		int br, br_lin=1; 
		char putanjaDoTxt[80], linija[80];
		close(pd1[1]);
		close(pd2[0]);
		read(pd1[0], &br, sizeof(int));
		printf("Drugi proces je primio duzinu: %d\n", br);
		read(pd1[0], putanjaDoTxt, 80);
		printf("Drugi proces je primio putanju: %s\n", putanjaDoTxt);
		fajl=fopen(putanjaDoTxt, "r");
		if(fajl==NULL)
		{
			printf("Ne mogu da otvorim fajl\n");
			return -1;
		}	
		while(fgets(linija, 80, fajl)!=NULL)
		{
			linija[strcspn(linija, "\n")]='\0';
			if(strlen(linija)>br)
				write(pd2[1], &br_lin, sizeof(int));
			br_lin++;
		}
		close(pd1[0]);
		close(pd2[1]);	
		fclose(fajl);
	}
}
