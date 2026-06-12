# include "result.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

Result create_error(ErrorType error_type, int error_code, const char * format, ...)
{
    Result error;

    error.status = ERROR;

    error.type = error_type;

    if (error.type == ERROR_TYPE_DATABASE)
    {
        error.code.database = error_code;
    } 
    else if (error.type == ERROR_TYPE_JSON)
    {
        error.code.json = error_code;
    }
    else 
    {
        fprintf(stderr, "\n THE ERROR TYPE YOU USED DOES NOT EXIST IN THE ERROR TYPE ENUM. \n");
        fflush(stderr);
        abort();   
    }

    va_list args;

    va_start(args, format);

    vsnprintf(
        error.message,
        sizeof(error.message),
        format,
        args);


    return error;
}

Result create_success()
{
    Result success;

    success.status = SUCCESS;

    return success;
}