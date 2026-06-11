#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef enum
{
    JSON_SUCCESS = 1,
    JSON_KEY_MISSING = -1,
    JSON_EMPTY = -2,
    JSON_MALFORMED = -3
} JsonStatus;

JsonStatus extract_json_value(const char *json, const char *key,
                       char *output, size_t output_size);


#endif