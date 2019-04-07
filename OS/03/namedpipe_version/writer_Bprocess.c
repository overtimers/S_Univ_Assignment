#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MSG_SIZE 512
#define PIPENAME "./named_pipe_file"

int main()
{
	char msg[MSG_SIZE];
	int fd;
	int ret,i;

	if(access(PIPENAME, F_OK) != 0) // create named pipe if it doesn't exist
	{
		if(mkfifo(PIPENAME, 0666) < 0)
		{
			printf("pipe create failed\n");
			return 0;
		}
	}

	fd = open(PIPENAME, O_WRONLY); // open named pipe
	if(fd < 0)
	{
		printf("Open failed\n");
		return 0;
	}

	while (1)
	{
		fgets(msg, MSG_SIZE, stdin); // get message from stdin
		ret = write(fd, msg, sizeof(msg)); // write message to pipe
		if (ret < 0)
		{
			printf("Write failed\n");
			break;
		}
		if (strcmp(msg, "exit\n")==0) // exit when the message is "exit"
			break;
	}
	unlink(PIPENAME); // unlink before return
	return 0;
}

