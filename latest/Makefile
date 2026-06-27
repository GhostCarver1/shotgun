shotgun:
	gcc logic/game/*.c logic/objects/*.c server/main.c server/sender.c server/result/*.c server/handlers/authentication/*.c server/handlers/games/*.c server/helpers/*.c database/database.c \
    	-I/usr/include/postgresql \
    	-o shotgun \
    	-lpq \
    	-lsodium

test:
	gcc logic/game/*.c logic/objects/*.c tests/*.c server/sender.c server/result/*.c server/handlers/authentication/*.c server/handlers/games/*.c server/helpers/*.c database/database.c \
    	-I/usr/include/postgresql \
    	-o test \
    	-lpq \
    	-lsodium
clean:
	rm -f *.o shotgun

rebuild: clean shotgun

clean_test:
	rm -f *.o test

rebuild_test: clean_test test

.PHONY: clean rebuild clean_test rebuild_test