#include <stdio.h>
#include <unistd.h>

int main()
{
	FILE* fp;
	int i;
	char buf[256];
	for(i=0; i<6; i++)
	{
		sleep(2);
		fp = fopen("test.txt", "r"); //open test.txt in r mode
		fseek(fp, 0, SEEK_SET); //set file pointer to beginning of file
		fgets(buf, 256, fp); //read string from file
		printf("%s",buf); //print read string
		fclose(fp);
	}
	return 0;
}

