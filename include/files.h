#ifndef FILES_H
#define FILES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define WRITE  "w"
#define APPEND "a"
#define READ   "r"

#define FILENAME     "./storage/todos.csv"
#define ARCHIVE_FILE "./storage/archive.csv"
#define LOG_FILE     "./storage/log.txt"

bool write_to_file(const char *filename, const char *text, const char *mode);
void read_from_file(const char *filename);

bool remove_line_from_file(const char *filename, const char *target);

#endif
