#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define COUNTING_NUMBER 10

void* writer(void*);
void* reader();

sem_t reader_sem;
sem_t writer_sem;

int cur_writer=0;
int cur_count=0;

int main()
{
	int i;
	int arr[2] = {0,1};
	pthread_t reader_threads[5];
	pthread_t writer_threads[2];
	
	sem_init(&reader_sem, 0, 5);
	sem_init(&writer_sem, 0, 1);

	for (i=0; i<5; i++)
		pthread_create(&reader_threads[i], NULL, reader, NULL);
	
	for (i=0; i<2; i++)
		pthread_create(&writer_threads[i], NULL, writer, &arr[i]);

	
	for (i=0; i<5; i++)
		pthread_join(reader_threads[i], NULL);
	
	for (i=0; i<2; i++)
		pthread_join(writer_threads[i], NULL);

	return 0;
}

void* writer(void* a)
{
	int i, j;
	int n = *(int*)a;
	for(i=0; i<COUNTING_NUMBER; i++)
	{
		usleep(1000);
		// lock
		sem_wait(&writer_sem);
		for(j=0; j<5;j++)
			sem_wait(&reader_sem);
		// critical section
		cur_writer = n;
		cur_count++;
		// unlock
		sem_post(&writer_sem);
		for(j=0; j<5;j++)
			sem_post(&reader_sem);
	}
}

void* reader()
{
	int i;
	for(i=0; i<COUNTING_NUMBER; i++)
	{
		usleep(300);
		// lock
		sem_wait(&writer_sem);
		sem_wait(&reader_sem);
		// critical section
		printf("cur_count: %d\tcur_writer: %d\n", cur_count, cur_writer);
		// unlock
		sem_post(&writer_sem);
		sem_post(&reader_sem);
	}
}

