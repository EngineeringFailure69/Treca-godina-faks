#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>

#define MAX_NAME 1024
void pronadjiDatoteku(char* putanja, char* ime);

int main(int argc, char* argv[])
{
	char dir_path[MAX_NAME];
	char filename[MAX_NAME];
	if(argc<3)
	{
		printf("Koriscenje: <direktorijum> <naziv_datoteke>\n");
		return 1;
	}
	strcpy(dir_path, argv[1]);
	strcpy(filename, argv[2]);

	pronadjiDatoteku(dir_path, filename);
	return 0;
}

void pronadjiDatoteku(char* dir_path, char* filename)
{
	DIR* dir;
	struct dirent* dirp;
	struct stat statbuf;
	char path[MAX_NAME];

	dir=opendir(dir_path);
	if(dir==NULL)
	{
		printf("Greska prilikom otvaranja direktorijuma\n");
		exit(-1);
	}

	while((dirp=readdir(dir))!=NULL)
	{
		if(strcmp(dirp->d_name, ".")==0 || strcmp(dirp->d_name, "..")==0)
			continue;
		
		strcpy(path, dir_path);
		strcat(path, "/");
		strcat(path, dirp->d_name);
		
		stat(path, &statbuf);
		
		if(S_ISREG(statbuf.st_mode) && strcmp(dirp->d_name, filename)==0)
		{
			printf("Pronadjena datoteka: %s\n", path);
			printf("Njena velicina je: %ld bajta\n", statbuf.st_size);
		}
		else if(S_ISDIR(statbuf.st_mode))
			pronadjiDatoteku(path, filename);
	}
	closedir(dir);	
}
