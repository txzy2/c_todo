#include "../include/files.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define READ_LENGTH 100

bool write_to_file(const char *filename, const char *text, const char *mode)
{
	if (filename == NULL || text == NULL || mode == NULL)
	{
		return false;
	}

	FILE *f = fopen(filename, mode);
	if (f == NULL)
	{
		return false;
	}

	fprintf(f, "%s", text);

	if (text[0] != '\0' && text[strlen(text) - 1] != '\n')
	{
		fprintf(f, "\n");
	}

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

bool remove_line_from_file(const char *filename, const char *target)
{
	if (filename == NULL || target == NULL)
	{
		return false;
	}

	FILE *src = fopen(filename, READ);
	if (src == NULL)
	{
		return false;
	}

	char line[512];
	char target_copy[512];
	char temp_filename[] = "./storage/temp.csv";

	snprintf(target_copy, sizeof(target_copy), "%s", target);
	target_copy[strcspn(target_copy, "\r\n")] = '\0';

	if (!write_to_file(temp_filename, "", "w"))
	{
		fclose(src);
		return false;
	}

	bool found = false;

	while (fgets(line, sizeof(line), src) != NULL)
	{
		line[strcspn(line, "\r\n")] = '\0';

		if (strcmp(line, target_copy) == 0)
		{
			found = true;
			continue;
		}

		if (!write_to_file(temp_filename, line, APPEND))
		{
			fclose(src);
			return false;
		}
	}

	fclose(src);

	if (!found || remove(filename) != 0)
	{
		remove(temp_filename);
		return false;
	}

	if (rename(temp_filename, filename) != 0)
	{
		return false;
	}

	return true;
}