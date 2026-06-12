shotgun:
	gcc latest/server/main.c latest/server/handlers/*.c latest/server/helpers/*.c latest/database/database.c \
    	-I/usr/include/postgresql \
    	-o shotgun \
    	-lpq \
    	-lsodium
