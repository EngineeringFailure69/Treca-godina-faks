#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<dirent.h>

#define MAX_LENGTH 100

void stampaj(char* putanja, int m)
{
	int i;
	char linija[MAX_LENGTH], path[1024];
	FILE* fajl=fopen(putanja, "r");
	if(fajl==NULL)
	{
		printf("Ne mogu da otvorim fajl\n");
		return;
	}

	for(i=0;i<m && fgets(linija, sizeof(linija), fajl);i++)
		printf("%s\n", linija);
	fclose(fajl);
}

int main(int argc, char* argv[])
{
	int brojac=0;
	char path[1024];
	DIR* dir;
	struct stat statbuf;
	struct dirent* dirp;
	
	if(argc!=4)
	{
		printf("Upotreba: <putanja_direktorijum> <n> <m>\n");
		return -1;
	}

	char* putanjaDoDir=argv[1];
	int n=atoi(argv[2]);
	int m=atoi(argv[3]);

	dir=opendir(putanjaDoDir);
	if(!dir)
	{
		printf("Greska pri otvaranju direktorijuma\n");
		return -1;
	}

	while((dirp=readdir(dir))!=NULL && brojac<n)
	{
		strcpy(path, argv[1]);
		strcat(path, "/");
		strcat(path, dirp->d_name);
		
		stat(path, &statbuf);
		
		if(S_ISREG(statbuf.st_mode) && stat(path, &statbuf)==0)
		{
			printf("Datoteka: %s\n", dirp->d_name);
			stampaj(path, m);
			brojac++;
		}		
	}
	closedir(dir);
	return 0;
}
