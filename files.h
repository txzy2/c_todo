#ifndef FILES_H
#define FILES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define FILENAME "./storage/todos.csv"

bool write_to_file(const char *filename, const char *text, const char *mode);
void read_from_file(const char *filename);

#endif
