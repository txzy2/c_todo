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
	const char *statuses[] = {"TODO", "DONE", "CANCELED", "DELETED"};

	for (int i = 0; i < 3; i++)
	{
		if (strcmp(str, statuses[i]) == 0)
		{
			return (enum Status)i;
		}
	}

	return UNKNOWN;
}

void print_header(void)
{
	printf("\n ID  STATUS  DATE        TITLE\n");
	printf("---  ------  ----------  ----------------------------\n");
}

void print_item(Item *item)
{
	if (item == NULL)
	{
		return;
	}
	char *status = enum_to_string(item->status);
	char *color = item->status == DONE ? GREEN : item->status == TODO ? YELLOW : RED;

	printf(" %-2d  %s%-6s" RESET "  %-10s  %-.28s\n", item->id, color, status, item->date, item->title);
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

void get_menu(void)
{
	printf(CYAN "\n-------- MENU --------\n" RESET " 1  Add Item\n"
	            " 2  Delete Item\n"
	            " 3  Edit status\n"
	            " 0  Exit\n" CYAN "-----------------------\n" RESET);
}
