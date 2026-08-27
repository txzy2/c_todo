#include "../include/files.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define READ_LENGTH 100
char FILENAME[MAX_PATH_LEN];
char ARCHIVE_FILE[MAX_PATH_LEN];
char LOG_FILE[MAX_PATH_LEN];
char TEMP_FILE[MAX_PATH_LEN];

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

	snprintf(target_copy, sizeof(target_copy), "%s", target);
	target_copy[strcspn(target_copy, "\r\n")] = '\0';

	if (!write_to_file(TEMP_FILE, "", WRITE))
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

		if (!write_to_file(TEMP_FILE, line, APPEND))
		{
			fclose(src);
			return false;
		}
	}

	fclose(src);

	if (!found)
	{
		remove(TEMP_FILE);
		return false;
	}

	if (remove(filename) != 0)
	{
		remove(TEMP_FILE);
		return false;
	}

	if (rename(TEMP_FILE, filename) != 0)
	{
		return false;
	}

	return true;
}

void init_storage_paths(void)
{
	const char *override = getenv("TXTODO_HOME");
	char base_dir[MAX_PATH_LEN];
	int res;

	if (override != NULL)
	{
		res = snprintf(base_dir, sizeof(base_dir), "%s", override);
	}
	else
	{
		const char *home = getenv("HOME");
		if (home == NULL)
		{
			home = ".";
		}
		res = snprintf(base_dir, sizeof(base_dir), "%s/.local/share/%s", home, STORAGE_DIR_NAME);
	}

	if (res < 0 || (size_t)res >= sizeof(base_dir))
	{
		fprintf(stderr, "Error: storage base path too long\n");
		exit(EXIT_FAILURE);
	}

	mkdir(base_dir, 0755);

	res = snprintf(FILENAME, sizeof(FILENAME), "%s/todos.csv", base_dir);
	if (res < 0 || (size_t)res >= sizeof(FILENAME))
	{
		fprintf(stderr, "Error: storage path too long\n");
		exit(EXIT_FAILURE);
	}

	res = snprintf(ARCHIVE_FILE, sizeof(ARCHIVE_FILE), "%s/archive.csv", base_dir);
	if (res < 0 || (size_t)res >= sizeof(ARCHIVE_FILE))
	{
		fprintf(stderr, "Error: archive path too long\n");
		exit(EXIT_FAILURE);
	}

	res = snprintf(LOG_FILE, sizeof(LOG_FILE), "%s/log.txt", base_dir);
	if (res < 0 || (size_t)res >= sizeof(LOG_FILE))
	{
		fprintf(stderr, "Error: log path too long\n");
		exit(EXIT_FAILURE);
	}

	res = snprintf(TEMP_FILE, sizeof(TEMP_FILE), "%s/temp.csv", base_dir);
	if (res < 0 || (size_t)res >= sizeof(TEMP_FILE))
	{
		fprintf(stderr, "Error: temp path too long\n");
		exit(EXIT_FAILURE);
	}
}
