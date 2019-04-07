#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main()
{
	int shmid;
	void *shmaddr;
	int ret;

	shmid = shmget((key_t)1234, 512, IPC_CREAT|0666); //get shared memory (key = 1234)
	if (shmid == -1)
	{
		perror("shared Memory access failed\n");
		return 0;
	}
	while(1)
	{
		shmaddr = shmat(shmid, (void*)0,  0); // attach shared memory
		if (shmaddr == (char *)-1)
		{
			perror("attach failed\n");
			return 0;
		}

		if(strcmp((char*)shmaddr, "") != 0) // when string is not empty, print the string
		{
			printf("%s", (char*)shmaddr);
			if (strcmp((char*)shmaddr, "exit\n") == 0) // when read "exit" return
			{
				if(shmdt(shmaddr) == -1) // detach memory
				{
					perror("detach failedn");
					return 0;
				}
				break;
			}
			strcpy((char*)shmaddr, ""); // make string empty after print
		}
		if(shmdt(shmaddr) == -1) // detach memory
		{
			perror("detach failedn");
			return 0;
		}
	}
	if (shmctl(shmid, IPC_RMID, 0) == -1) // remove shared memory
	{
		perror("remove failed\n");
		return 0;
	}
	return 0;
}
