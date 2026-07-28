#include "json_helper.h"

int is_letter(char character)
{
    return character<=122 && character>=48;
}

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
        return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_MALFORMED, "THE BUFFER WAS TO SMALL TO FIT THE ENTRY");
    }

    strncpy(output, start, value_length);
    output[value_length] = '\0';

    return create_success();
}

Result extract_json_list_of_strings(const char *json, const char *key, size_t list_size, size_t buffer_size, char output[list_size][buffer_size])
{

    //json has to be exactly \"key\"":["%s","%s",...] or will fail

    //first, make all the outputs safe by null terminating them all

    for (int i = 0; i < list_size; i++)
    {
        output[i][buffer_size-1] = '\0';
        output[i][0] = '\0';
    }

    char pattern[64];
    snprintf(pattern, sizeof(pattern), "\"%s\":[", key);
    char *abs_start = strstr(json, pattern);
    if (abs_start==NULL)
    {
        return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_KEY_MISSING, "COULD NOT FIND THE PATTERN \':\"%s\":[\' \n", key);
    }
    abs_start += strlen(pattern);
    
    char *abs_end = strstr(abs_start, "]");

    if (abs_end==NULL)
    {
        return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_KEY_MISSING, "COULD NOT FIND CLOSING BRACKET FOR KEY %s \n", key);
    }

    // if the '[   ]' is empty then dont continue

    if (abs_start == abs_end)
    {
        return create_success();
    }



    // check to see if the array is empty

    abs_start += 1;


    int end = 0;
    int index = 0;
    while(index < list_size && !end)
    {
        int i;
        for (i=0;i<buffer_size && is_letter(abs_start[i]); i++)
        {
            if (i == buffer_size - 1)
            {
                return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_MALFORMED, "ONE OF THE BUFFERS WAS TO SMALL TO FIT ONE OF THE ENTRIES");
            }
            output[index][i] = abs_start[i];
        }
        output[index][i] = '\0';


        if (abs_start[i]!='"')
        {
            return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_MALFORMED, "UNESPECTED CHARACTER AT THE END OF JSON LIST: '%c' is not character paranthases",abs_start[i]);
        }
        
        if (abs_start[i+1]==']')
        {
            end = 1;
            break;
        }
        if (abs_start[i+1]==','&&abs_start[i+2]=='"')
        {
            abs_start += (i+3);
            index++;
        }
        else 
        {
            return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_MALFORMED, "UNESPECTED CHARACTER AT THE END OF JSON LIST %c%c",abs_start[i+2],abs_start[i+3]);
        }
        if (index == list_size)
        {
             return create_error(ERROR_TYPE_JSON, ERROR_CODE_JSON_MALFORMED, "NUMBER OF STRINGS DETECTED IS TO MUCH FOR THE BUFFER TO HANDLE \n");
        }
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