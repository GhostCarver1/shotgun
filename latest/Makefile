shotgun:
	gcc server/main.c tests/*.c server/result/*.c server/handlers/authentication/*.c server/helpers/*.c database/database.c \
    	-I/usr/include/postgresql \
    	-o shotgun \
    	-lpq \
    	-lsodium

clean:
	rm -f *.o shotgun

rebuild: clean shotgun

.PHONY: clean rebuild