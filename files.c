#include "files.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READ_LENGTH 100

size_t read_from_file_with_array(const char *filename, char arr[][30])
{
	FILE *rf = fopen(filename, "r");

	if (rf == NULL)
	{
		return 0;
	}

	size_t line = 0;

	while (line < 100 && fgets(arr[line], 30, rf))
	{
		arr[line][strcspn(arr[line], "\r\n")] = '\0';
		line++;
	}

	fclose(rf);

	return line;
}

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
