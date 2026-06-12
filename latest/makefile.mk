shotgun:
	gcc server/main.c server/result/*.c server/handlers/*.c server/helpers/*.c database/database.c \
    	-I/usr/include/postgresql \
    	-o shotgun \
    	-lpq \
    	-lsodium

clean:
	rm -f *.o shotgun
