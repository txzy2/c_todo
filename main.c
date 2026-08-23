#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_TIME 30

enum Status
{
	TODO = 0,
	DONE,
	CANCELED,
};

typedef struct
{
	int id;
	char *title;
	char *desc;
	enum Status status;
} Item;

/* ======= STORAGE ======= */

typedef struct
{
	Item **data;
	int size;
	uint32_t cap;
} Storage;

bool init_storage(Storage *st, uint32_t cap);
bool load_storage(Storage *st);
void move_into_storage(Storage *st, Item *item);

void clear_storage(Storage *st);

/* ======= UTILS ======= */

void get_time(char *buff, size_t size);
const char *get_status(enum Status s);
void print_item(Item *item);

/* ======= ITEMS ======= */

char *get_input(const char *msg);
Item *create_item();

bool create(Storage *st); // GET INPUT + CERATE ITEM + MOVE ITEM INTO STORAGE
bool delete(Storage *st, int id);
bool find(Storage *st, int id);

bool change_status(Storage *st, int id, enum Status s);

/* ======= MAIN ======= */

void prepoccess(Storage *st)
{
	if (!init_storage(st, 1))
	{
		fprintf(stderr, "Error: failed to initialize storage\n");
		abort();
	}

	if (!load_storage(st))
	{
		fprintf(stderr, "Error: failed to load storage\n");
		abort();
	}
}

int main(void)
{
	int result = EXIT_SUCCESS;

	Storage st;
	prepoccess(&st);

	printf("OK\n");

	goto cleanup;

cleanup:
	clear_storage(&st);
	return result;
}

/**
 * =============================================================================
 * FUNCTIONS
 * This is a list of functions that are used in the program.
 * =============================================================================
 */

/* ======= UTILS ======= */

void get_time(char *buff, size_t size)
{
	time_t now = time(NULL);
	struct tm *cur = localtime(&now);

	snprintf(buff, size, "%02d.%02d.%04d | %02d:%02d:%02d", cur->tm_mday, cur->tm_mon + 1, cur->tm_year + 1900,
	         cur->tm_hour, cur->tm_min, cur->tm_sec);
}

/* ======= STORAGE ======= */

bool init_storage(Storage *st, uint32_t cap)
{
	if (st == NULL || cap == 0)
	{
		return false;
	}

	st->data = malloc(cap * sizeof(*st->data));
	if (st->data == NULL)
	{
		return false;
	}

	st->size = 0;
	st->cap = cap;

	return true;
}

bool load_storage(Storage *st) { return true; }

void clear_storage(Storage *st)
{
	if (st->data != NULL)
	{
		for (int i = 0; i < st->size; i++)
		{
			free(st->data[i]);
		}

		free(st->data);
	}

	st->data = NULL;
	st->size = st->cap = 0;
}
