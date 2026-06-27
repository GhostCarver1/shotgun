#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../result/result.h"

#define FIELD(name, request) {#name, request.name, sizeof(request.name)}

typedef struct {
    char * key;
    char * output;
    size_t output_size;
} JsonFeild;

Result extract_json_value(const char *json, const char *key,
                       char *output, size_t output_size);

Result extract_json_list_of_strings(const char *json, const char *key, size_t list_size, size_t buffer_size, char output[list_size][buffer_size]);

Result extract_json_feilds(const char *json, JsonFeild * json_feilds, size_t count);


#endif