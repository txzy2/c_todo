#ifndef FILES_H
#define FILES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define WRITE  "w"
#define APPEND "a"
#define READ   "r"

#define STORAGE_DIR_NAME "txtodo"
#define MAX_PATH_LEN     512

void init_storage_paths(void);

extern char FILENAME[MAX_PATH_LEN];
extern char ARCHIVE_FILE[MAX_PATH_LEN];
extern char LOG_FILE[MAX_PATH_LEN];

bool write_to_file(const char *filename, const char *text, const char *mode);
void read_from_file(const char *filename);

bool remove_line_from_file(const char *filename, const char *target);

#endif
