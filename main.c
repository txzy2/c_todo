#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/files.h"
#include "include/items.h"
#include "include/storage.h"
#include "include/utils.h"

atomic_bool running = true;

void *timer_thread(void *arg)
{
	(void)arg;

	char timer[BUFFER_TIME];

	while (atomic_load(&running))
	{
		get_time(timer, sizeof(timer));

		printf("\033[s");
		printf("\033[1;1H");
		printf("%s", timer);
		printf("\033[u");

		fflush(stdout);

		sleep(1);
	}

	return NULL;
}

bool preprocess(Storage *st)
{
	if (!init_storage(st, 1))
	{
		fprintf(stderr, "Error: failed to initialize storage\n");
		return false;
	}

	if (!load_storage(st))
	{
		fprintf(stderr, "Error: failed to load storage\n");
		return false;
	}

	return true;
}

int main(void)
{
	init_storage_paths();

	int result = EXIT_FAILURE;
	bool is_running = true;
	pthread_t timer;

	if (pthread_create(&timer, NULL, timer_thread, NULL) != 0)
	{
		return EXIT_FAILURE;
	}

	Storage st = {0};
	if (!preprocess(&st))
	{
		goto cleanup;
	}

	while (is_running)
	{
		if (clear_terminal())
		{
			if (st.size > 0)
			{
				print_header();

				int printed = print_items_by_status(&st, TODO);
				if (printed > 0)
				{
					printf(DIM "------------------------------------------------------\n" RESET);
				}

				printed = print_items_by_status(&st, WORK);
				if (printed > 0)
				{
					printf(DIM "------------------------------------------------------\n" RESET);
				}

				print_items_by_status(&st, DONE);
			}
		}

		get_menu();
		char choice[10];
		get_input("Choice", choice, sizeof(choice));

		switch (atoi(choice))
		{
			case 1:
			{
				if (!create(&st))
				{
					fprintf(stderr, "Error: failed to create item\n");
					break;
				}

				result = EXIT_SUCCESS;
				break;
			}
			case 2:
			{
				char id[10];
				get_input("Paste ID:", id, sizeof(id));
				if (atoi(id) == 0)
				{
					fprintf(stderr, "INVALID INPUT\n");
					break;
				}

				if (!delete_item(&st, atoi(id)))
				{
					break;
				}

				printf("ITEM DELETED\n");
				result = EXIT_SUCCESS;
				break;
			}
			case 3:
			{
				char id[10];
				get_input("Paste ID:", id, sizeof(id));
				if (atoi(id) == 0)
				{
					fprintf(stderr, "INVALID INPUT\n");
					break;
				}

				Item *item = find_item(&st, atoi(id));
				if (item == NULL)
				{
					fprintf(stderr, "ITEM NOT FOUND\n");
				}
				else
				{
					print_item_details(item);
				}

				printf("\nPress Enter to continue...");
				getchar();
				break;
			}
			case 4:
			{
				char id[10];
				get_input("Paste ID:", id, sizeof(id));
				if (atoi(id) == 0)
				{
					fprintf(stderr, "INVALID INPUT\n");
					break;
				}

				Item *item = find_item(&st, atoi(id));
				if (item == NULL)
				{
					fprintf(stderr, "ITEM NOT FOUND\n");
				}

				char status[STATUS_LENGTH];
				if (!get_input("Status ('DONE', 'TODO', 'WORK', 'CANCELED', 'ARCHIVE'): ", status, sizeof(status)))
				{
					fprintf(stderr, "INVALID INPUT\n");
					break;
				}

				enum Status s = string_to_enum(status);
				if (s == UNKNOWN)
				{
					fprintf(stderr, "INVALID STATUS\n");
					break;
				}

				if (!change_status(&st, atoi(id), s))
				{
					fprintf(stderr, "ERROR CONVERT STATUS\n");
					break;
				}

				break;
			}
			case 0:
			default:
				is_running = false;
		}

		if (!is_running)
		{
			printf("Bye-bye! Press Enter to continue...");
			getchar();
			clear_terminal();
		}
	}

	result = EXIT_SUCCESS;

cleanup:
	atomic_store(&running, false);
	pthread_join(timer, NULL);

	clear_terminal();
	clear_storage(&st);

	return result;
}
