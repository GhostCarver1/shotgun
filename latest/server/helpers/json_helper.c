#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sodium.h>
#include "json_helper.h"
#include "../result/result.h"

Result extract_json_value(const char *json, const char *key,
                       char *output, size_t output_size)
{
    if (strcmp(json, "{}")==0 || strcmp(json, "{ }") == 0)
    {
        return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_EMPTY, "THE JSON WAS COMPLETELEY EMPTY WHEN TRYING TO GET VALUES ");
    }
    char pattern[64];

    snprintf(pattern, sizeof(pattern), "\"%s\":\"", key);

    char *start = strstr(json, pattern);

    if (!start)
    {
        return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_KEY_MISSING, "MISSING JSON KEY: %s ", key);
    }

    start += strlen(pattern);

    char *end = strchr(start, '"');

    if (!end)
    {
        fprintf(stderr, "Malformed JSON for key: %s\n", key);
        return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_MALFORMED, "MALFORMED JSON FOR KEY: %s ", key);
    }

    size_t value_length = end - start;

    if (value_length >= output_size)
    {
        value_length = output_size - 1;
    }

    strncpy(output, start, value_length);
    output[value_length] = '\0';

    return create_success();
}