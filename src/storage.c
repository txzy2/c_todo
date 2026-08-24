#include "../include/storage.h"
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
