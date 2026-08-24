#ifndef STORAGE_H
#define STORAGE_H

#include "../include/todo.h"

bool init_storage(Storage *st, uint32_t cap);
bool load_storage(Storage *st);
bool move_into_storage(Storage *st, Item *item);
void clear_storage(Storage *st);

#endif
