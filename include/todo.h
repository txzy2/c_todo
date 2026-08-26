#ifndef TODO_H
#define TODO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BUFFER_TIME      30
#define TITLE_LENGTH     50
#define DESC_LENGTH      256
#define STATUS_LENGTH    10
#define LINE_BUFFER_SIZE (TITLE_LENGTH + DESC_LENGTH + STATUS_LENGTH + BUFFER_TIME + 32)

enum Status
{
	TODO = 0,
	DONE,
	WORK,
	CANCELED,
	DELETED,
	UNKNOWN
};

typedef struct
{
	int id;
	char title[TITLE_LENGTH];
	char desc[DESC_LENGTH];
	char date[BUFFER_TIME];
	enum Status status;
} Item;

typedef struct
{
	Item **data;
	int size;
	uint32_t cap;
} Storage;

#endif
