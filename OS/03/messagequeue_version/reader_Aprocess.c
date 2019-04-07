#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct msgbuf
{
	long msgtype;
	char mtext[256];
};

int main()
{
	key_t key_id;
	const int key=1234;
	struct msgbuf rsvbuf;
	int msgtype=3;
	char recvmsg[256];

	key_id= msgget((key_t)key, IPC_CREAT|0666);

	if (key_id == -1)
	{
		perror("msgget error : ");
		return 0;
	}
	while(1)
	{
		if (msgrcv(key_id, (void*)&rsvbuf, sizeof(rsvbuf), msgtype, 0) == -1)
		{
			perror("msgrcv error : ");
		}
		else
		{
			strcpy(recvmsg, rsvbuf.mtext);
			printf("%s", recvmsg);
			if(strcmp(recvmsg, "exit\n") == 0)
				break;
		}
	}
	return 0;
}


