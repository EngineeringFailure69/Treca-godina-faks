#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include<time.h>
#include<sys/file.h>

int main()
{
	int pd1[2], pd2[2], pid1, pid2, i, broj;
	
	srand(time(NULL));
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
	
	if(pid1=fork()!=0)
	{
		close(pd1[0]);
		close(pd2[0]);
		close(pd2[1]);
		
		for(i=0;i<10;i++)
		{
			broj=rand()%99;
			printf("Random broj koji je prvi proces poslao drugom procesu: %d\n", broj);
			write(pd1[1], &broj, sizeof(int));
		}
		
		wait(NULL);
		close(pd1[1]);
		printf("Prvi proces izlazi\n");
		return 0;
	}
	
	if(pid2=fork()!=0)
	{
		int primljen;
		close(pd1[1]);
		close(pd2[0]);
		for(i=0;i<10;i++)
		{
			read(pd1[0], &primljen, sizeof(int));
			printf("Broj koji je drugi proces primio je: %d\n", primljen);
			if(primljen%2==0)
				primljen/=2;
			printf("Broj koji je drugi proces poslao trecem procesu: %d\n", primljen);
			write(pd2[1], &primljen, sizeof(int));
		}
		
		wait(NULL);
		close(pd1[0]);
		close(pd2[1]);
		printf("Drugi proces izlazi\n");
		exit(0);
	}
	
	else
	{
		int primljen2, fd;
		close(pd1[0]);
		close(pd1[1]);
		close(pd2[1]);
		
		fd=open("redirekcija", O_CREAT|O_TRUNC|O_RDWR, 0777);
		for(i=0;i<10;i++)
		{
			read(pd2[0], &primljen2, sizeof(int));
			if(primljen2>10)
			{
				printf("Broj koji je treci proces primio je: %d\n", primljen2);
				dup2(fd, 1);
			}
		}
		
		close(fd);
		wait(NULL);
		close(pd2[0]);
		printf("Treci proces izlazi\n");
		exit(0);
	}
	return 0;
}
