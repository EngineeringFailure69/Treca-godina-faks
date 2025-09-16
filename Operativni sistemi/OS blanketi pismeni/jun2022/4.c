#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<sys/stat.h>
#include<time.h>
#include<ctype.h>

#define MAX_NAME 1024
void processdir(char* path, int level);

int main(int argc, char* argv[])
{
	DIR* dp;
	struct stat statbuf;
	char dirname[MAX_NAME];
	struct dirent* dirp;
	char path1[MAX_NAME];
	int level=1;
	if(argc!=2)
	{
		printf("Morate proslediti putanju do direktorijuma, i ime datoteke");
		exit(-1);
	}

	strcpy(dirname, argv[1]);
	
	processdir(dirname, 1);
	return 0;
}

void processdir(char* path, int level)
{
	DIR* dp;
	struct stat statbuf;
	char dirname[MAX_NAME];
	struct dirent* dirp;
	char path1[MAX_NAME];
	
	if((dp=opendir(path))==NULL)
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

		if(strstr(dirp->d_name, "log")!=NULL && S_ISREG(statbuf.st_mode) && statbuf.st_size>15*1024)
		{
			if(fork()==0)
			{
				printf("Delete file: %s\n", path1);
				execlp("rm", "rm", path1, NULL);
			}
			else
			{
				wait(NULL);
			}
		}
		else if(S_ISDIR(statbuf.st_mode) && level<3)
			processdir(path1, level+1);
	}
	closedir(dp);
}
