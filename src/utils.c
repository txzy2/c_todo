#include "../include/utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void clear_terminal(void)
{
	printf("\033[2J\033[H");
	fflush(stdout);
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
	default:
	case UNKNOWN:
		return "UNKNOWN";
	}
}

enum Status string_to_enum(const char *str)
{
	const char *statuses[] = {"TODO", "DONE", "CANCELED"};

	for (int i = 0; i < 3; i++)
	{
		if (strcmp(str, statuses[i]) == 0)
		{
			return (enum Status)i;
		}
	}

	return UNKNOWN;
}

void print_item(Item *item)
{
	if (item == NULL)
	{
		return;
	}

	char *status = enum_to_string(item->status);
	char *color = item->status == DONE ? GREEN : item->status == TODO ? YELLOW : RED;

	printf("| ID: %d\n| Title: %s\n| Description: %s\n| Status: "
	       "%s%s" RESET "\n| DATE: %s\n",
	       item->id, item->title, item->desc, color, status, item->date);
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
