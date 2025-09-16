#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/file.h>
#include<unistd.h>

#define MAX 80

int main()
{
	int pd1[2], pd2[2], pid;
	char trazenaRec[MAX];
	FILE* fajl;
	
	if(pipe(pd1)==-1)
	{
		printf("Greska prilikom kreiranja prvog datavoda\n");
		return -1;
	}
	if(pipe(pd2)==-1)
	{
		printf("Greska prilikom kreiranja drugog datavoda\n");
		return -1;
	}
	
	pid=fork();
	if(pid>0)
	{
		char odgovor[80];
		close(pd2[1]);
		close(pd1[0]);
		
		while(1)
		{
			printf("Unesite rec koju trazimo: ");
			fgets(trazenaRec, MAX, stdin);
			trazenaRec[strcspn(trazenaRec, "\n")]='\0';
			if(strcmp(trazenaRec, "END")==0)
			{
				break;
			}
			write(pd1[1], trazenaRec, strlen(trazenaRec)+1);
			printf("Proces roditelj je poslao rec: %s\n", trazenaRec);
			read(pd2[0], odgovor, MAX);
			if(strcmp(odgovor, "FOUND")==0)
				printf("Trazena rec se nalazi u fajlu reci.txt\n");
			else //if(strcmp(odgovor, "NOT FOUND")==0)
				printf("Trazena rec se ne nalazi u fajlu reci.txt\n");
		}//while(1);//(strcmp(trazenaRec, "END")!=0);
		
		printf("Uneli ste rec: END, prvi proces izlazi, i program se zavrsava\n");
		close(pd1[1]);
		close(pd2[0]);
		wait(NULL);
		//return -1;
	}	
	else
	{
		char trazenaRec2[MAX], linija[MAX], odgovor[MAX];
		close(pd2[0]);
		close(pd1[1]);
		
		fajl=fopen("reci", "r");
		if(fajl==NULL)
		{
			printf("Ne mogu da otvorim fajl\n");
			return -1;
		}
		
		while(1)//do
		{
			int nadjena=0;
			read(pd1[0], trazenaRec2, MAX);
			printf("Proces dete je primio rec: %s\n", trazenaRec2);	
			if(strcmp(trazenaRec2, "END")==0)
			{
				break;
			}
			
			while(fgets(linija, MAX, fajl)!=NULL)
			{
				linija[strcspn(linija, "\n")]='\0';
				if(strcmp(trazenaRec2, linija)==0)
				{
					nadjena=1;
					break;
					//strcpy(odgovor, "FOUND");
					//printf("Odgovor je: %s\n", odgovor);
					//write(pd2[1], odgovor, strlen(odgovor)+1);
				}
				//else //if(strcmp(trazenaRec2, linija)!=0)
				//{
				//	strcpy(odgovor, "NOT FOUND");
				//	printf("Odgovor je: %s\n", odgovor);
				//	write(pd2[1], odgovor, strlen(odgovor)+1);
				//}
			}
			rewind(fajl);
			
			if(nadjena)
				strcpy(odgovor, "FOUND");
			else
				strcpy(odgovor, "NOT FOUND");
			write(pd2[1], odgovor, strlen(odgovor)+1);
		}//while(1);//(strcmp(trazenaRec2, "END")!=0);
		
		printf("Uneli ste rec: END, drugi proces izlazi, i program se zavrsava\n");
		fclose(fajl);
		close(pd1[0]);
		close(pd2[1]);
		return -1;
	}
	return 0;
}
