#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <atomic>

#define COUNTING_NUMBER 20000000
int critical_section_variable = 0;
std::atomic_int turn(0);
std::atomic_int flag[2];

void lock(int self)
{
	int other = !self;
	flag[self] = 1;
	turn = other;
	while(flag[other] && turn == other) ;
}
void unlock(int self)
{
	flag[self] = 0;
}

void* func (void *s)
{
	int* thread_num = (int*) s;
	int i;
	for(i=0; i<COUNTING_NUMBER; i++)
	{
		lock(*thread_num);
		critical_section_variable++;
		unlock(*thread_num);
	}
}

int main(void)
{
	pthread_t p1, p2;
	int parameter[2] = {0, 1};
	flag[0] = 0;
	flag[1] = 0;

	pthread_create(&p1, NULL, func, (void*)&parameter[0]);
	pthread_create(&p2, NULL, func, (void*)&parameter[1]);

	pthread_join(p1,NULL);
	pthread_join(p2,NULL);

	printf("Actual Count: %d | Expected Count: %d\n",
			critical_section_variable,
			COUNTING_NUMBER*2);
	return 0;
}
