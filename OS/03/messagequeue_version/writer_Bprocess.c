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
	const int key=1234; //message queue key
	struct msgbuf sndbuf;

	key_id = msgget((key_t)key, IPC_CREAT|0666); //open message queue

	if (key_id == -1)
	{
		perror("msgget error: ");
		return 0;
	}

	sndbuf.msgtype = 3;
	while(1)
	{
		fgets(sndbuf.mtext, 256, stdin); //read from stdin
		if(msgsnd(key_id, (void*)&sndbuf, sizeof(sndbuf), IPC_NOWAIT) == -1) // write to message queue
		{
			perror("msgsnd error: ");
			return 0;
		}
		if (strcmp(sndbuf.mtext, "exit\n") == 0) // if write "exit" exit
			break;
	}
	return 0;
}

