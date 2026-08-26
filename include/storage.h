#ifndef STORAGE_H
#define STORAGE_H

#include "../include/todo.h"

bool init_storage(Storage *st, uint32_t cap);
bool load_storage(Storage *st);
bool move_into_storage(Storage *st, Item *item);
bool move_into_archive(char *str);
void clear_storage(Storage *st);
bool save_storage_to_file(Storage *st);

int find_index_by_id(const Storage *st, int id);
Item *find_item(Storage *st, const int id);

bool delete_item(Storage *st, const int id);

#endif
