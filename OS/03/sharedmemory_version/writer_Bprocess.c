#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main()
{
	int shmid;
	int i;
	void *shmaddr;
	char msg[512];
	
	shmid = shmget((key_t)1234, 512, IPC_CREAT|0666);
	if (shmid <0)
	{
		perror("shmget failed\n");
		return 0;
	}

	while(1)
	{
		shmaddr = shmat(shmid, (void*)0, 0);
		fgets(msg, 512, stdin);
		if (shmaddr == (char*)-1)
		{
			perror("attach failed\n");
			return 0;
		}
		strcpy((char*)shmaddr, msg);

		if (strcmp(msg, "exit\n") == 0)
			break;

		if(shmdt(shmaddr) == -1)
		{
			perror("detach failed\n");
			return 0;
		}
	}
	return 0;
}




