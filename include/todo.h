#ifndef TODO_H
#define TODO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BUFFER_TIME 30

enum Status
{
	TODO = 0,
	DONE,
	CANCELED,
	DELETED,
	UNKNOWN
};

typedef struct
{
	int id;
	char title[50];
	char desc[100];
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
