#include <stdio.h>
#include <unistd.h>

int main()
{
	pid_t child1, child2;
	int stat1, stat2;

	if ((child1 = fork()) == 0) //fork child1
	{
		execl("./child1", NULL); //execute child1
	}
	else if ((child2 = fork()) == 0) //fork child2 from parent
	{
		execl("./child2", NULL); //execute child2
	}
	else //parent process
	{
		printf("waiting for child process\n");
		// wait child processes
		if (waitpid(child1, &stat1, 0) != child1)
			printf("waitpid error\n");
		if (waitpid(child2, &stat2, 0) != child2)
			printf("waitpid error\n");
		printf("child processes are exit (%d) (%d)\n", stat1, stat2);
	}
	return 0;
}
