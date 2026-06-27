#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sodium.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#include "main.h"
#include "constants.h"
#include "../database/database.h"
#include "helpers/json_helper.h"

#include "handlers/authentication/permission.h"
#include "handlers/authentication/signup.h"
#include "handlers/authentication/login.h"
#include "sender.h"


int main(int argc, char ** argv)
{
    sodium_init();
    setup_webpage();
}

