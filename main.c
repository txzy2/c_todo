#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "files.h"

#define BUFFER_TIME 30

atomic_bool running = true;

enum Status
{
	TODO = 0,
	DONE,
	CANCELED,
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

/* ======= STORAGE ======= */

typedef struct
{
	Item **data;
	int size;
	uint32_t cap;
} Storage;

bool init_storage(Storage *st, uint32_t cap);
bool load_storage(Storage *st);
bool move_into_storage(Storage *st, Item *item);

void clear_storage(Storage *st);

/* ======= UTILS ======= */

void clear_terminal(void);
bool prepoccess(Storage *st);

void get_time(char *buff, size_t size);
enum Status string_to_enum(const char *str);
char *enum_to_string(const enum Status s);
void print_item(Item *item);

/* ======= ITEMS ======= */

bool get_input(const char *msg, char *buff, size_t size);
Item *create_item(const char *title, const char *desc, const enum Status status, Storage *st);

bool create(Storage *st); // GET INPUT + CERATE ITEM + MOVE ITEM INTO STORAGE
bool delete(Storage *st, int id);
bool find(Storage *st, int id);

bool change_status(Storage *st, int id, enum Status s);

/* ======= MAIN ======= */

void *timer_thread(void *arg)
{
	(void)arg;

	char timer[BUFFER_TIME];

	while (atomic_load(&running))
	{
		get_time(timer, sizeof(timer));

		printf("\033[s");    // сохранить позицию курсора
		printf("\033[1;1H"); // перейти в начало
		printf("%s", timer);
		printf("\033[u"); // восстановить позицию курсора

		fflush(stdout);

		sleep(1);
	}

	return NULL;
}

int main(void)
{
	clear_terminal();

	int result = EXIT_FAILURE;
	pthread_t timer;

	if (pthread_create(&timer, NULL, timer_thread, NULL) != 0)
	{
		return EXIT_FAILURE;
	}

	Storage st;
	if (!prepoccess(&st))
	{
		goto cleanup;
	}

	if (!create(&st))
	{
		goto cleanup;
	}

	for (int i = 0; i < st.size; ++i)
	{
		print_item(st.data[i]);
	}

	result = EXIT_SUCCESS;

cleanup:
	atomic_store(&running, false);
	pthread_join(timer, NULL);

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

void clear_terminal(void)
{
	printf("\033[2J\033[H");
	fflush(stdout);
}

bool prepoccess(Storage *st)
{
	if (!init_storage(st, 1))
	{
		fprintf(stderr, "Error: failed to initialize storage\n");
		return false;
	}

	// if (!load_storage(st))
	// {
	// 	fprintf(stderr, "Error: failed to load storage\n");
	// 	return false;
	// }

	return true;
}

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

bool move_into_storage(Storage *st, Item *item)
{
	if (st == NULL || item == NULL)
	{
		return false;
	}

	if (st->cap == (uint32_t)st->size)
	{
		uint32_t new_cap = st->cap * 2;
		Item **tmp = realloc(st->data, new_cap * sizeof(*st->data));
		if (tmp == NULL)
		{
			return false;
		}

		st->data = tmp;
		st->cap = new_cap;
	}

	st->data[st->size++] = item;
	return true;
}

/**
 * =============================================================================
 * ITEMS
 * This is a list of functions that are used to create, delete, find, change
 * status of items.
 * =============================================================================
 */

void print_item(Item *item)
{
	if (item == NULL)
	{
		return;
	}

	char *status = enum_to_string(item->status);

	printf("| ID: %d\n| Title: %s\n| Description: %s\n| Status: %s\n| DATE: %s\n", item->id, item->title, item->desc,
	       status, item->date);
}

bool get_input(const char *msg, char *buff, size_t size)
{
	if (msg == NULL || buff == NULL || size == 0)
	{
		return false;
	}

	printf("%s\n", msg);
	printf("> ");

	if (fgets(buff, size, stdin) == NULL)
	{
		return false;
	}

	buff[strcspn(buff, "\n")] = '\0';

	return true;
}

char *enum_to_string(const enum Status s)
{
	switch (s)
	{
	case TODO:
		return "TODO";
	case DONE:
		return "DONE";
	case CANCELED:
		return "CANCELED";
	default:
	case UNKNOWN:
		return "UNKNOWN";
	}
}

enum Status string_to_enum(const char *str)
{
	const char *statuses[] = {"TODO", "DONE", "CANCELED"};

	for (int i = 0; i < 3; i++)
	{
		if (strcmp(str, statuses[i]) == 0)
		{
			return (enum Status)i;
		}
	}

	return UNKNOWN;
}

Item *create_item(const char *title, const char *desc, const enum Status status, Storage *st)
{
	Item *item = malloc(sizeof(Item));
	if (item == NULL)
	{
		return NULL;
	}

	item->id = st->size + 1;

	strncpy(item->title, title, sizeof(item->title) - 1);
	item->title[sizeof(item->title) - 1] = '\0';

	strncpy(item->desc, desc, sizeof(item->desc) - 1);
	item->desc[sizeof(item->desc) - 1] = '\0';

	item->status = status;

	char date[BUFFER_TIME];
	get_time(date, BUFFER_TIME);

	strncpy(item->date, date, sizeof(item->date) - 1);
	item->date[sizeof(item->date) - 1] = '\0';

	return item;
}

bool create(Storage *st)
{
	if (st == NULL)
	{
		return false;
	}

	char title[50];
	char desc[100];
	char status[10];

	if (!get_input("Title (max 49 chars): ", title, sizeof(title)))
	{
		return false;
	}

	if (!get_input("Description (max 99 chars): ", desc, sizeof(desc)))
	{
		return false;
	}

	if (!get_input("Status ('DONE', 'TODO', 'CANCELED'): ", status, sizeof(status)))
	{
		return false;
	}

	enum Status s = string_to_enum(status);

	if (s == UNKNOWN)
	{
		fprintf(stderr, "Error: unknown status\n");
		return false;
	}

	Item *item = create_item(title, desc, s, st);

	if (item == NULL)
	{
		return false;
	}

	if (!move_into_storage(st, item))
	{
		free(item);
		return false;
	}

	return true;
}
