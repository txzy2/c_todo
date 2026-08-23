.PHONY: b r check

init:
	mkdir -p bin && mkdir -p storage

b:
	gcc -Wall -Wextra -Werror -pedantic -std=c11 -o bin/main main.c files.c

r: b
	./a.out

check: b
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/main
