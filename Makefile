.PHONY: b r check install uninstall

BINARY_NAME = txtodo
INSTALL_DIR = $(HOME)/.local/bin

DEV_STORAGE = $(CURDIR)/storage

init:
	mkdir -p bin

b:
	gcc -std=c11 -O2 -Wall -Wextra -Wpedantic -Iinclude -Ilib/logger -pthread main.c src/files.c src/storage.c src/utils.c src/items.c lib/logger/logger.c -o bin/$(BINARY_NAME)

r: b
	TXTODO_HOME=$(DEV_STORAGE) ./bin/$(BINARY_NAME)

check: b
	TXTODO_HOME=$(DEV_STORAGE) valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/$(BINARY_NAME)
install: b
	mkdir -p $(INSTALL_DIR)
	cp bin/$(BINARY_NAME) $(INSTALL_DIR)/$(BINARY_NAME)
	@echo "Установлено в $(INSTALL_DIR)/$(BINARY_NAME)"
	@echo "Убедись, что $(INSTALL_DIR) есть в $$PATH"

uninstall:
	rm -f $(INSTALL_DIR)/$(BINARY_NAME)
