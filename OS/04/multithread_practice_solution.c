#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARGUMENT_NUMBER 20

long long result[ARGUMENT_NUMBER] = {0,}; // array for save results

void *ThreadFunc(void *n)
{
	long long i;
	long long number = *((long long *)n);
	printf("number = %lld\n", number);
	for (i=0; i<25000000; i++)
	{
		result[number] += number;
	}
}

int main(void)
{
	long long argument[ARGUMENT_NUMBER];
	pthread_t threadID[ARGUMENT_NUMBER];
	long long i;
	long long r = 0;
	long long t;
	for (i=0; i<ARGUMENT_NUMBER; i++)
	{
		argument[i] = i;
		
		//create thread and call ThreadFunc
		pthread_create(&threadID[i], NULL, ThreadFunc, (void*)&argument[i]);
	}
	printf("Main Thread is waiting for Child Thread!\n");
	for (i=0; i<ARGUMENT_NUMBER; i++)
	{
		//wait for threads
		pthread_join(threadID[i], NULL);
	}
	for (i=0; i<ARGUMENT_NUMBER; i++)
		r = r + result[i];
	printf("result = %lld\n", r);
	return 0;
}

