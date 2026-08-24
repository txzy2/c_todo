#include "../include/storage.h"
#include "../include/files.h"
#include "../include/items.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

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

bool load_storage(Storage *st)
{
	if (st == NULL)
	{
		return false;
	}

	FILE *fptr = fopen(FILENAME, "r");
	if (fptr == NULL)
	{
		return false;
	}
	char buffStr[256];
	while (fgets(buffStr, sizeof(buffStr), fptr))
	{
		char id_str[20], title[100], desc[50], status_str[10], date[BUFFER_TIME];

		if (sscanf(buffStr, "%19[^;];%99[^;];%49[^;];%9[^;];%29[^;\n]", id_str, title, desc, status_str, date) == 5)
		{
			enum Status status = string_to_enum(status_str);

			Item *item = create_item(title, desc, status, date, st);
			if (item == NULL)
			{
				fprintf(stderr, "Memory allocation failed\n");
				continue;
			}

			if (!move_into_storage(st, item))
			{
				free(item);
			}
		}
	}
	fclose(fptr);
	return true;

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
