#include "../include/utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

bool clear_terminal(void)
{
	printf("\033[2J\033[H");
	fflush(stdout);

	return true;
}

void get_time(char *buff, size_t size)
{
	time_t now = time(NULL);
	struct tm *cur = localtime(&now);

	snprintf(buff, size, "%02d.%02d.%04d | %02d:%02d:%02d", cur->tm_mday, cur->tm_mon + 1, cur->tm_year + 1900,
	         cur->tm_hour, cur->tm_min, cur->tm_sec);
}

char *enum_to_string(const enum Status s)
{
	switch (s)
	{
		case TODO:
			return "TODO";
		case DONE:
			return "DONE";
		case WORK:
			return "WORK";
		case CANCELED:
			return "CANCELED";
		case DELETED:
			return "DELETED";
		default:
		case UNKNOWN:
			return "UNKNOWN";
	}
}

enum Status string_to_enum(const char *str)
{
	const char *statuses[] = {"TODO", "DONE", "WORK", "CANCELED", "DELETED"};

	for (int i = 0; i < 5; i++)
	{
		if (strcmp(str, statuses[i]) == 0)
		{
			return (enum Status)i;
		}
	}

	return UNKNOWN;
}

bool get_input(const char *msg, char *buff, size_t size)
{
	if (msg == NULL || buff == NULL || size == 0)
	{
		return false;
	}

	printf("%s\n", msg);
	printf("> ");

	if (fgets(buff, (int)size, stdin) == NULL)
	{
		return false;
	}

	buff[strcspn(buff, "\n")] = '\0';

	return true;
}

const char *status_color(enum Status s)
{
	switch (s)
	{
		case DONE:
			return GREEN;
		case TODO:
			return YELLOW;
		case WORK:
			return CYAN;
		case CANCELED:
		case DELETED:
			return RED;
		default:
			return RESET;
	}
}

char status_symbol(enum Status s)
{
	switch (s)
	{
		case DONE:
			return 'v';
		case WORK:
			return '~';
		case TODO:
			return '*';
		case CANCELED:
		case DELETED:
			return 'x';
		default:
			return '?';
	}
}

void print_header(void)
{
	printf("\n ID   STATUS  DATE        TITLE\n");
	printf("----  ------  ----------  ----------------------------\n");
}

void print_item(Item *item)
{
	if (item == NULL)
	{
		return;
	}
	char *status = enum_to_string(item->status);
	const char *color = status_color(item->status);
	char sym = status_symbol(item->status);

	char date_tmp[BUFFER_TIME];
	get_date(date_tmp, sizeof(date_tmp), item);

	printf(GREEN " %-2d" RESET "  %s%c %-4s" RESET "  " DIM "%-10s" RESET "  " BOLD "%-.50s" RESET "\n", item->id,
	       color, sym, status, date_tmp, item->title);
}

int print_items_by_status(Storage *st, enum Status status)
{
	int n = 0;

	for (int i = 0; i < st->size; ++i)
	{
		if (st->data[i]->status == status)
		{
			print_item(st->data[i]);
			n++;
		}
	}

	return n;
}

void get_menu(void)
{
	printf(CYAN "\n--------------- MENU ----------------\n" RESET " 1  Add Item\n"
	            " 2  Delete Item\n"
	            " 3  View Item\n"
	            " 4  Change Status\n"
	            " 0  Exit\n" CYAN "-------------------------------------\n" RESET);
}

void print_item_details(Item *item)
{
	if (item == NULL)
	{
		return;
	}

	char *status = enum_to_string(item->status);
	const char *color = status_color(item->status);
	char sym = status_symbol(item->status);

	printf("\n" MAGENTA "-------------- ITEM #%d --------------" RESET "\n" MAGENTA "Title:      " RESET " " BOLD
	       "%s" RESET "\n" MAGENTA "Description:" RESET " %s\n" MAGENTA "Status:     " RESET " %s%c %s" RESET
	       "\n" MAGENTA "Date:       " RESET " " DIM "%s" RESET "\n" MAGENTA
	       "-------------------------------------" RESET "\n",
	       item->id, item->title, item->desc, color, sym, status, item->date);
}
