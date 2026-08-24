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

	return true;
}

int main(void)
{
	clear_terminal();

	int result = EXIT_FAILURE;
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

	if (!create(&st))
	{
		goto cleanup;
	}

	printf("\n");
	for (int i = 0; i < st.size; ++i)
	{
		printf("==== ITEM ====\n");
		print_item(st.data[i]);
		printf("==============\n");
	}

	result = EXIT_SUCCESS;

cleanup:
	atomic_store(&running, false);
	pthread_join(timer, NULL);

	clear_storage(&st);

	return result;
}
