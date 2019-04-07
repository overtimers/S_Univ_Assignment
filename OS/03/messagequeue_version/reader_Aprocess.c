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
	const int key=1234; // message queue key
	struct msgbuf rsvbuf;
	int msgtype=3;
	char recvmsg[256];

	key_id= msgget((key_t)key, IPC_CREAT|0666); // open message queue

	if (key_id == -1)
	{
		perror("msgget error : ");
		return 0;
	}
	while(1)
	{
		if (msgrcv(key_id, (void*)&rsvbuf, sizeof(rsvbuf), msgtype, 0) == -1) // read message from message queue
		{
			perror("msgrcv error : ");
		}
		else
		{
			strcpy(recvmsg, rsvbuf.mtext); // copy recieved message to recvmsg
			printf("%s", recvmsg);
			if(strcmp(recvmsg, "exit\n") == 0) // when read "exit" exit
				break;
		}
	}
	return 0;
}


