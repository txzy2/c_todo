#ifndef UTILS_H
#define UTILS_H

#include "../include/todo.h"

void clear_terminal(void);
void get_time(char *buff, size_t size);
enum Status string_to_enum(const char *str);
char *enum_to_string(const enum Status s);
void print_item(Item *item);
bool get_input(const char *msg, char *buff, size_t size);

#endif
