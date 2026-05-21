#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sodium.h>
#include "json_helper.h"

JsonStatus extract_json_value(const char *json, const char *key,
                       char *output, size_t output_size)
{
    if (strcmp(json, "{}")==0 || strcmp(json, "{ }") == 0)
    {
        return JSON_EMPTY;
    }
    char pattern[64];

    snprintf(pattern, sizeof(pattern), "\"%s\":\"", key);

    char *start = strstr(json, pattern);

    if (!start)
    {
        fprintf(stderr, "Missing JSON key: %s\n", key);
        return JSON_KEY_MISSING;
    }

    start += strlen(pattern);

    char *end = strchr(start, '"');

    if (!end)
    {
        fprintf(stderr, "Malformed JSON for key: %s\n", key);
        return JSON_MALFORMED;
    }

    size_t value_length = end - start;

    if (value_length >= output_size)
    {
        value_length = output_size - 1;
    }

    strncpy(output, start, value_length);
    output[value_length] = '\0';

    return JSON_SUCCESS;
}