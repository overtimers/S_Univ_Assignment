#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

sem_t chopsticks[5];

void* philosopher(void* n)
{
	int i = *(int*) n;
	while(1)
	{
		//THINKING
		printf("philosopher %d: Thinking\n", i);
		
		//HUNGRY
		//picking chopsticks
		if ( i % 2 == 0) // L type philosopher
		{
			sem_wait(&chopsticks[i]);
			sem_wait(&chopsticks[(i+1)%5]);
		}
		else // R type philosopher
		{
			sem_wait(&chopsticks[(i+1)%5]);
			sem_wait(&chopsticks[i]);
		}
		// EATING
		printf("philosopher %d: Eating\n", i);
		//putting chopsticks
		sem_post(&chopsticks[i]);
		sem_post(&chopsticks[(i+1)%5]);
	}
}


int main()
{
	int i;
	pthread_t philosophers[5];
	int arg[5] = {0,1,2,3,4};
	
	for (i=0; i<5; i++)
		sem_init(&chopsticks[i], 0, 1);

	for (i=0; i<5; i++)
		pthread_create(&philosophers[i], NULL, philosopher, &arg[i]);
	
	for (i=0; i<5; i++)
		pthread_join(philosophers[i], NULL);
	for (i=0; i<5; i++)
		sem_destroy(&chopsticks[i]);

	return 0;
}

