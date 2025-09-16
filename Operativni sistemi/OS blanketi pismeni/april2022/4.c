#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>

int main(int argc, char* argv[])
{
	if(argc!=3)
	{
		printf("Upotreba: %s <direktorijum1> <direktorijum2>\n", argv[0]);
		return 1;
	}

	char* directory1 = argv[1];
	char* directory2 = argv[2];

	DIR *dir1, *dir2;
	struct dirent *entry1, *entry2;
	struct stat stat1, stat2;

	dir1=opendir(directory1);
	if(dir1==NULL)
	{
		printf("Nije moguce otvoriti prvi direktorijum\n");
		closedir(dir1);
		return 1;
	}

	dir2=opendir(directory2);
	if(dir2==NULL)
	{
		printf("Nije moguce otvoriti drugi direktorijum\n");
		closedir(dir2);
		return 1;
	}

	while((entry1 = readdir(dir1))!=NULL)
	{
		if(entry1->d_type==DT_REG)
		{
			char file1_path[PATH_MAX];
			printf("%s/%s\n", directory1, entry1->d_name);

			if(stat(file1_path, &stat1)!=0)
			{
				printf("Greska pri citanju informacija o datoteci\n");
				continue;
			}

			rewinddir(dir2);

		 while((entry2 = readdir(dir2))!=NULL)
       		 {
               		 if(entry2->d_type==DT_REG)
               		 {
                       		 char file2_path[PATH_MAX];
                       		 printf("%s/%s\n", directory2, entry2->d_name);

                       		 if(stat(file2_path, &stat2)!=0)
                       		 {
                               		 printf("Greska pri citanju informacija o datoteci\n");
                               		 continue;
                       		 }
			       	if(strcmp(entry1->d_name, entry2->d_name)==0 && stat1.st_size == stat2.st_size)
				{
					printf("%s\n", entry1->d_name);
					break;
				}
		       	}
		}
	}
}

	closedir(dir1);
	closedir(dir2);
}
