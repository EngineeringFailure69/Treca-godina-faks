#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/file.h>

#define MAX_LENGTH 256

int main()
{
	int pid, pd[2];
	char trazenaRec[80];
	char putanjaDoTxt[80];
	FILE* f;

	if(pipe(pd)==-1)
	{
		printf("Greska prilikom kreiranja datavoda\n");
		return -1;
	}

	if(pid=fork()!=0)
	{
		close(pd[0]);
		printf("Unesite putanju do txt datoteke koju citamo:");
       		fgets(putanjaDoTxt, 80, stdin);
       		putanjaDoTxt[strcspn(putanjaDoTxt, "\n")]='\0'
       		printf("Unesite kljucnu rec koju trazimo: ");
	        fgets(trazenaRec, 80, stdin);
		write(pd[1], putanjaDoTxt, 80);
		write(pd[1], trazenaRec, 80);
		close(pd[1]);
	}
	else
	{
		char primljenaPutanja[80];
		char primljenaRec[80];
		int brojLinije=1;
		char linija[80];
		close(pd[1]);
		read(pd[0], primljenaPutanja, 80);
		printf("\nPrimljena putanja do txt datoteke je: %s\n", primljenaPutanja);
		read(pd[0], primljenaRec, 80);
		printf("Primljena kljucna rec je: %s\n", primljenaRec);
		f=fopen(primljenaPutanja, "r");
		while(fgets(linija, 80, f)!=NULL)
		{
			if(strstr(linija, primljenaRec)!=NULL)
				printf("Rec je pronadjena na liniji: %d\n", brojLinije);
			brojLinije++;
		}		
		close(pd[0]);
	}
	return 0;
}
