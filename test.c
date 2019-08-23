
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *args[])
{
	unsigned char end_token = EKWA_END;
	unsigned char *buffer;
	FILE *fp;

	if (argc != 2) {
		printf("Invalid args.\n");
	}

	fp = fopen(args[1], "wb");
	fseek(fp, 0, SEEK_END);

	fwrite(&end_token, 1, 1, fp);


}