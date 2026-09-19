#ifndef MAIN_H
#define MAIN_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int main();
int setup_webpage();
void * handle_response(void * arg);

#endif