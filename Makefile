.PHONY: b r check

init:
	mkdir -p bin storage

b:
	gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude -pthread main.c src/files.c src/storage.c src/utils.c src/items.c -o bin/main

r: b
	./bin/main

check: b
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/main
