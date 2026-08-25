#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
	int result = EXIT_FAILURE;
	bool is_running = true;
	pthread_t timer;

	if (pthread_create(&timer, NULL, timer_thread, NULL) != 0)
	{
		return EXIT_FAILURE;
	}

	Storage st;
	if (!preprocess(&st))
	{
		goto cleanup;
	}

	while (is_running)
	{
		if (clear_terminal())
		{
			printf("\n");
			for (int i = 0; i < st.size; ++i)
			{
				printf("==== ITEM ====\n");
				print_item(st.data[i]);
				printf("==============\n");
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
				goto cleanup;
			}

			result = EXIT_SUCCESS;
			goto cleanup;
		}
		case 2:
		{
			char id[10];
			get_input("Paste ID:", id, sizeof(id));
			if (atoi(id) == 0)
			{
				fprintf(stderr, "INVALID INPUT\n");
				goto cleanup;
			}

			if (!delete_item(&st, atoi(id)))
			{
				goto cleanup;
			}

			printf("ITEM DELETED\n");
			result = EXIT_SUCCESS;
			goto cleanup;
		}
		case 0:
		default:
			is_running = false;
		}

		if (!is_running)
		{
			printf("Bye-bye! Press Enter to continue...");
			getchar();
		}
	}

	result = EXIT_SUCCESS;

cleanup:
	atomic_store(&running, false);
	pthread_join(timer, NULL);

	clear_storage(&st);

	return result;
}
