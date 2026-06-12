#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../result/result.h"

Result extract_json_value(const char *json, const char *key,
                       char *output, size_t output_size);


#endif