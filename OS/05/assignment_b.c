#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>

#define COUNTING_NUMBER 20000000
struct message 
{
	int turn;
	int flag[2];
	int data;
};

void lock(struct message *msg, int num)
{
	if (num == 0 || num == 1)
	{
		msg->flag[num] = 1;
		msg->turn = !num;
		while (msg->flag[!num] && msg->turn == !num) ;
	}
	else
		return;
}

void unlock(struct message *msg, int num)
{
	msg->flag[num] = 0;
}



int main()
{
	// fork child processes
	pid_t pid1, pid2;
	int shmid;
	int count;
	int i, status;
	void *shmaddr;
	struct message *msg;
	if ( (pid1 = fork()) < 0)
		return 0;
	// child process 1
	else if (pid1 == 0)
	{
		//get shared memory id
		shmid = shmget(1234, sizeof(struct message), IPC_CREAT | 0666);
		if (shmid < 0)
		{
			perror("shared memory access failed\n");
			return 0;
		}
		count = 0;
		//attach shared memory
		shmaddr = shmat(shmid, (void*)0, NULL);
		msg = (struct message *) shmaddr;
		for (i=0; i<COUNTING_NUMBER; i++)
		{
			lock((struct message *) shmaddr, 0);
			count++;
			msg->data = msg->data + 1;
			unlock((struct message *) shmaddr, 0);
		}
		//detach shared memory
		shmdt(shmaddr);
		printf("process num : %d, local count : %d\n", 1, count);
		return 0;
	}
	else if ( (pid2 = fork()) < 0)
		return 0;
	// child process 2
	else if (pid2 == 0)
	{
		//get shared memory id
		shmid = shmget(1234, sizeof(struct message), IPC_CREAT | 0666);
		if (shmid < 0)
		{
			perror("shared memory access failed\n");
			return 0;
		}
		count = 0;
		//attach shared memory
		shmaddr = shmat(shmid, (void*)0, NULL);
		msg=(struct message *) shmaddr;
		for (i=0; i<COUNTING_NUMBER; i++)
		{
			lock((struct message *) shmaddr, 1);
			count++;
			msg->data++;
			unlock((struct message *) shmaddr, 1);
		}
		//detach shared memory
		shmdt(shmaddr);
		printf("process num : %d, local count : %d\n", 2, count);
		return 0;
	}
	// parent process
	else
	{
		shmid = shmget(1234, sizeof(struct message), IPC_CREAT | 0666);
		if (shmid < 0)
		{
			perror("shared memory access failed\n");
			return 0;
		}
		waitpid(pid1, &status, 0);
		printf("child %d finish!\n", pid1);
		waitpid(pid2, &status, 0);
		printf("child %d finish!\n", pid2);
		//attach shared memory
		shmaddr = shmat(shmid, (void*)0, NULL);
		msg = (struct message *) shmaddr;
		printf("Actual Count: %d | Expected Count: 40000000\n",msg->data);
		//detach and remove shared memory
		shmdt(shmaddr);
		shmctl(shmid, IPC_RMID, NULL);
		return 0;
	}

	return 0;
}

