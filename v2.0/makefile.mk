shotgun:
	gcc server/main.c server/handlers/*.c server/helpers/*.c database/database.c \
    	-I/usr/include/postgresql \
    	-o shotgun \
    	-lpq \
    	-lsodium
