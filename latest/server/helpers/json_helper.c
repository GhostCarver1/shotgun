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
        return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_EMPTY, "THE JSON WAS COMPLETELEY EMPTY WHEN TRYING TO GET VALUES");
    }
    char pattern[64];

    snprintf(pattern, sizeof(pattern), "\"%s\":\"", key);

    char *start = strstr(json, pattern);

    if (!start)
    {
        return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_KEY_MISSING, "MISSING JSON KEY: %s", key);
    }

    start += strlen(pattern);

    char *end = strchr(start, '"');

    if (!end)
    {
        fprintf(stderr, "Malformed JSON for key: %s\n", key);
        return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_MALFORMED, "MALFORMED JSON FOR KEY: %s", key);
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

Result extract_json_list_of_strings(const char *json, const char *key, size_t list_size, size_t buffer_size, char output[list_size][buffer_size])
{
    if (strcmp(json, "{}")==0 || strcmp(json, "{ }") == 0)
    {
        return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_EMPTY, "THE JSON WAS COMPLETELEY EMPTY WHEN TRYING TO GET VALUES");
    }
    char pattern[64];

    snprintf(pattern, sizeof(pattern), "\"%s\":[\"", key);

    char *abs_start = strstr(json, pattern);
    char *abs_end = strchr(abs_start, ']');

    if (!abs_start)
    {
        return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_KEY_MISSING, "MISSING JSON KEY: %s", key);
    }

    abs_start += strlen(pattern);

    if (!abs_start)
    {
        return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_MALFORMED, "MALFORMED KEY: %s", key);
    }   

    char *start = abs_start;
    char *end;
    int index = 0;

    while (index < list_size)
    {
        char *end = strchr(start, '"');

        printf("start: %s \n", start);
        printf("end: %s \n", end);


        if (!end)
        {
            fprintf(stderr, "Malformed JSON for key: %s\n", key);
            return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_MALFORMED, "MALFORMED JSON FOR KEY: %s", key);
        }
        
        if (end-start >= buffer_size)
        {
            fprintf(stderr, "Buffer Size could not fit the entire stirng: %s\n", key);
            return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_MALFORMED, "TO BIG TO FIT INTO THE BUFFER: %s", key);
        }
        
        strncpy(output[index], start, end-start);
        output[index++][end-start] = '\0';

        start = strchr(end+1, '"');

        if (!start)
            break;
        start++;
    }

    return create_success();

}



Result extract_json_feilds(const char *json, JsonFeild * json_feilds, size_t count)
{
    for (int i = 0; i < count; i++)
    {
        Result result = extract_json_value(json, json_feilds[i].key, json_feilds[i].output, json_feilds[i].output_size);
        if (result.status==ERROR)
        {
            return result;
        }
    }
    return create_success();
}