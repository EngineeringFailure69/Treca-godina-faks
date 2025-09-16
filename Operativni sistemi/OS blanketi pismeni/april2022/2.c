#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>

int main()
{
	int pd1[2], pd2[2], broj, i, pid, brojac=0;
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

	if(pid=fork()!=0)
	{
		close(pd1[0]);
		close(pd2[1]);
		for(i=0;i<10;i++)
		{
			broj=rand()%100+200;
			printf("Random broj koji je poslat detetu: %d\n", broj);
			write(pd1[1], &broj, sizeof(int));
		}
		int primljen;
		for(i=0;i<10;i++)
		{
			read(pd2[0], &primljen, sizeof(int));
			printf("Broj koji roditelj prima od deteta: %d\n", primljen);
		}
		close(pd1[1]);
		close(pd2[0]);
	}
	else
	{
		close(pd1[1]);
		close(pd2[0]);
		int primljen, poslat;
		for(i=0;i<10;i++)
		{
			read(pd1[0], &primljen, sizeof(int));
			printf("Broj koji je dete primilo od roditelja: %d\n", primljen);
			if(primljen%3==0)
			{
				brojac++;
				poslat=primljen+25;
				write(pd2[1], &poslat, sizeof(int));
				printf("Broj koji je dete poslalo roditelju posle modifikacije: %d\n", poslat);
			}
		}
		close(pd1[0]);
		close(pd2[1]);
	}

	return 0;
}
