#include "files.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define READ_LENGTH 100

bool write_to_file(const char *filename, const char *text, const char *mode)
{
	FILE *f = fopen(filename, mode);
	if (f == NULL)
	{
		return false;
	}

	fprintf(f, "%s\n", text);
	fclose(f);

	return true;
}

void read_from_file(const char *filename)
{
	FILE *rf = fopen(filename, "r");

	if (rf == NULL)
	{
		return;
	}

	char file_str[READ_LENGTH];
	printf("READ FROM FILE (<line-num> <line-data>): \n");
	int line = 1;
	while (fgets(file_str, READ_LENGTH, rf))
	{
		printf("%d %s", line, file_str);
		line++;
	}

	fclose(rf);
}
