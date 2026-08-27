#ifndef UTILS_H
#define UTILS_H

#include "../include/todo.h"

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"
#define RESET   "\033[0m"

bool clear_terminal(void);

void get_time(char *buff, size_t size);
void get_date(char *buff, size_t size);
enum Status string_to_enum(const char *str);
char *enum_to_string(const enum Status s);

const char *status_color(enum Status s);
char status_symbol(enum Status s);

void print_header(void);
void print_item(Item *item);
int print_items_by_status(Storage *st, enum Status status);
void print_item_details(Item *item);

bool get_input(const char *msg, char *buff, size_t size);

void get_menu();

#endif
