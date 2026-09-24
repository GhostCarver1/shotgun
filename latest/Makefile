CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -I/usr/include/postgresql
LDLIBS := -lpq -lsodium

COMMON_SOURCES := \
	logic/game/*.c \
	server/sender.c \
	server/result/*.c \
	server/handlers/authentication/*.c \
	server/handlers/games/*.c \
	server/helpers/*.c \
	database/database.c

shotgun:
	$(CC) $(CFLAGS) $(COMMON_SOURCES) server/main.c -o shotgun $(LDLIBS)

test:
	$(CC) $(CFLAGS) $(COMMON_SOURCES) tests/*.c -o test $(LDLIBS)

clean:
	rm -f *.o shotgun

clean_test:
	rm -f *.o test

rebuild: clean shotgun

rebuild_test: clean_test test

.PHONY: shotgun test clean clean_test rebuild rebuild_test