#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/file.h>
#include<sys/wait.h>

int main(int argc, char* argv[])
{
	int pid, pd1[2], pd2[2];
	char trazenaRec[80], putanjaDoTxt[80];
	FILE* fajl;
	
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
	
	if(pid=fork()!=0)
	{
		int brLinije;
		close(pd1[0]);
		close(pd2[1]);
		printf("Unesite putanju do txt datoteke koju citam: ");
		fgets(putanjaDoTxt, 80, stdin);
	      	putanjaDoTxt[strcspn(putanjaDoTxt, "\n")]='\0';
		printf("Unesite kljucnu rec koju trazim: ");
		fgets(trazenaRec, 80, stdin);
		write(pd1[1], putanjaDoTxt, 80);
		write(pd1[1], trazenaRec, 80);
		while(read(pd2[0], &brLinije, sizeof(int))>0)
			printf("Rec je nadjena na liniji: %d\n", brLinije);
		close(pd1[1]);
		close(pd2[0]);
		wait(NULL);
	}
	else
	{
		char primljenaPutanja[80], primljenaRec[80], linija[80];
		int brLinije=1;
		close(pd1[1]);
		close(pd2[0]);
		read(pd1[0], primljenaPutanja, 80);
		printf("\nPrimljena putanja do txt datoteke je: %s\n", primljenaPutanja);
		read(pd1[0], primljenaRec, 80);
		printf("Primljena kljucna rec je: %s\n", primljenaRec);
		fajl=fopen(primljenaPutanja, "r");
		if(fajl==NULL)
		{
			printf("Ne mogu da otvorim fajl\n");
			return -1;
		}
		while(fgets(linija, 80, fajl)!=NULL)
		{
			if(strstr(linija, primljenaRec)!=NULL)
				write(pd2[1], &brLinije, sizeof(int));
			brLinije++;
		}
		close(pd1[0]);
		close(pd2[1]);
	}
}
