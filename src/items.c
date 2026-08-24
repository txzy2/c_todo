#include "../include/items.h"
#include "../include/files.h"
#include "../include/storage.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: Сделать добавлене даты но не обязательным полем
Item *create_item(const char *title, const char *desc, const enum Status status, char *date, Storage *st)
{
	Item *item = malloc(sizeof(Item));
	if (item == NULL)
	{
		return NULL;
	}

	item->id = (st->size > 0) ? st->data[st->size - 1]->id + 1 : 1;

	strncpy(item->title, title, sizeof(item->title) - 1);
	item->title[sizeof(item->title) - 1] = '\0';

	strncpy(item->desc, desc, sizeof(item->desc) - 1);
	item->desc[sizeof(item->desc) - 1] = '\0';

	item->status = status;

	if (date == NULL)
	{
		get_time(item->date, sizeof(item->date));
	}
	else
	{
		strncpy(item->date, date, sizeof(item->date) - 1);
		item->date[sizeof(item->date) - 1] = '\0';
	}

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

	Item *item = create_item(title, desc, s, NULL, st);

	if (item == NULL)
	{
		return false;
	}

	if (!move_into_storage(st, item))
	{
		free(item);
		return false;
	}

	char buff[100];
	char date_tmp[BUFFER_TIME];

	strncpy(date_tmp, item->date, sizeof(date_tmp) - 1);
	date_tmp[sizeof(date_tmp) - 1] = '\0';

	char *p = strchr(date_tmp, '|');
	if (p != NULL)
	{
		*p = '\0';
	}

	int res = snprintf(buff, sizeof(buff), "%d;%s;%s;%s;%s", item->id, item->title, item->desc,
	                   enum_to_string(item->status), date_tmp);

	if (res < 0 || (size_t)res > sizeof(buff))
	{
		return false;
	}

	return write_to_file(FILENAME, buff, "a");
}

bool delete(Storage *st, int id)
{
	(void)st;
	(void)id;
	return false;
}

bool find(Storage *st, int id)
{
	(void)st;
	(void)id;
	return false;
}

bool change_status(Storage *st, int id, enum Status s)
{
	(void)st;
	(void)id;
	(void)s;
	return false;
}
