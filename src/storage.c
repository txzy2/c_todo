#include "../include/storage.h"
#include "../include/files.h"
#include "../include/items.h"
#include "../include/utils.h"
#include <linux/limits.h>
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

bool delete_item(Storage *st, const int id)
{
	if (st == NULL || id <= 0 || st->size == 0)
	{
		return false;
	}

	int new_size = st->size - 1;
	Item **buff_data = malloc(new_size * sizeof(*st->data));
	if (buff_data == NULL && new_size > 0)
	{
		return false;
	}

	int j = 0;
	for (int i = 0; i < st->size; i++)
	{
		if (st->data[i]->id == id)
		{
			free(st->data[i]);
		}
		else
		{
			// NOTE: Если не нашли то просто пока точ заполняем буффер
			buff_data[j++] = st->data[i];
		}
	}

	// NOTE: Не нашли вообще ничего то просто чистим фуфер и отправляем warning
	if (j == st->size)
	{
		free(buff_data);
		fprintf(stderr, "DELETE ITEM NOT FOUND\n");
		return true;
	}

	free(st->data);
	st->data = buff_data;
	st->size = new_size;

	if (!write_to_file(FILENAME, "", WRITE))
	{
		fprintf(stderr, "ERROR WRITE TO FILE\n");
		return false;
	}

	for (int i = 0; i < st->size; i++)
	{
		char buff[256];
		Item *item = st->data[i];
		snprintf(buff, sizeof(buff), "%d;%s;%s;%s;%s", item->id, item->title, item->desc, enum_to_string(item->status),
		         item->date);

		if (!write_to_file(FILENAME, buff, APPEND))
		{
			fprintf(stderr, "ERROR APPEND TO FILE\n");
			return false;
		}
	}

	return true;
}