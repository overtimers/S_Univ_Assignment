#include <stdio.h>
#include <unistd.h>

int main()
{
	FILE * fp; 
	int i;
	for (i = 0; i < 12; i++)
	{
		sleep(1);
		fp = fopen("test.txt", "w"); //open test.txt in w mode
		fseek(fp, 0, SEEK_SET); //set file pointer to beginning of file
		fprintf(fp, "%d\n", i); //write i in test.txt
		printf("child1 wrote %d.\n", i); //print what is written
		fclose(fp);
	}
	return 0;
}

		
