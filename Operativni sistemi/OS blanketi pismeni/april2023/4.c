#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>

#define MAX_NAME 1024
void processdir(char* path, int level);
char datoteka[MAX_NAME];

int main(int argc, char* argv[])
{
	char dirname[MAX_NAME];
	char path[MAX_NAME];
	struct stat statbuf;
	DIR* dp;
	struct dirent* dirp;

	if(argc<2)
	{
		printf("Nema dovoljno ulaznih argumenata\n");
		exit(-1);
	}

	strcpy(dirname, argv[1]);

	if(stat(dirname, &statbuf)<0)
	{
		printf("Greska prilikom ocitavanja statusa unete putanje\n");
		exit(-1);
	}

	if(!S_ISDIR(statbuf.st_mode))
	{
		printf("Uneti argument ne predstavlja putanju do direktorijuma\n");
		exit(-1);
	}

	processdir(dirname, 1);
	printf("Najveca datoteka je: %s\n", datoteka);
	return 0;
}

void processdir(char* path, int level)
{
	DIR* dp;
	struct dirent *dirp;
	struct stat statbuf;
	int max_size=0;
	char path1[MAX_NAME];

	dp=opendir(path);
	if(dp==NULL)
	{
		printf("Greska prilikom otvaranja direktorijuma\n");
		exit(-1);
	}

	while((dirp=readdir(dp))!=NULL)
	{		
		strcpy(path1, path);
		strcat(path1, "/");
		strcat(path1, dirp->d_name);

		stat(path1, &statbuf);

		if(S_ISREG(statbuf.st_mode) && statbuf.st_size > max_size)
		{
			max_size=statbuf.st_size;
							
			strcpy(datoteka, dirp->d_name);
		}
		else if(S_ISDIR(statbuf.st_mode) && level<5)
			processdir(path1, level+1);
	}
	closedir(dp);
}
