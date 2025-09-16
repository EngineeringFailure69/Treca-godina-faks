#include<stdio.h>
#include<pthread.h>

pthread_mutex_t mutex;
pthread_cond_t uslovna1, uslovna2;

FILE* f;

void* velikaSlova()
{
	int i;
	
	pthread_mutex_lock(&mutex);
	for(i='A';i<='Z';i++)
	{
		pthread_cond_wait(&uslovna2, &mutex);
		f = fopen("tekst.txt", "a");
		fprintf(f, "%c\n", i);
		fclose(f);
		//fflush(stdout);
		//sleep(1);
		pthread_cond_signal(&uslovna1);		
	}
	pthread_mutex_unlock(&mutex);	
}

void* malaSlova()
{
	int i;
	
	pthread_mutex_lock(&mutex);
	for(i='a';i<='z';i++)
	{
		f = fopen("tekst.txt", "a");
		fprintf(f, "%c\n", i);
		fclose(f);
		//fflush(stdout);
		//sleep(1);
		pthread_cond_signal(&uslovna2);
		pthread_cond_wait(&uslovna1, &mutex);
	}
	pthread_mutex_unlock(&mutex);
}

void main()
{
	pthread_t mala, velika;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&uslovna1, NULL);
	pthread_cond_init(&uslovna2, NULL);

	pthread_create(&mala, NULL, malaSlova, NULL);
	//sleep(3);
	pthread_create(&velika, NULL, velikaSlova, NULL);

	pthread_join(mala, NULL);
	pthread_join(velika, NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&uslovna1);
	pthread_cond_destroy(&uslovna2);
}
