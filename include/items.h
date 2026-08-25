#ifndef ITEMS_H
#define ITEMS_H

#include "../include/todo.h"

Item *create_item(const char *title, const char *desc, const enum Status status, char *date, Storage *st);
bool create(Storage *st);
bool change_status(Storage *st, int id, enum Status s);

#endif
