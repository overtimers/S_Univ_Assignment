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

	if(access(PIPENAME, F_OK) != 0)
	{
		if(mkfifo(PIPENAME, 0666) < 0)
		{
			printf("pipe create failed\n");
		}
	}

	fd = open(PIPENAME, O_WRONLY);
	if(fd < 0)
	{
		printf("Open failed\n");
		return 0;
	}

	while (1)
	{
		fgets(msg, MSG_SIZE, stdin);
		ret = write(fd, msg, sizeof(msg));
		if (ret < 0)
		{
			printf("Write failed\n");
			return 0;
		}
		if (strcmp(msg, "exit\n")==0)
			break;
	}
	unlink(PIPENAME);
	return 0;
}

