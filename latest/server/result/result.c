# include "result.h"
#include <stdio.h>
#include <stdlib.h>

Result create_error(ErrorType error_type, int error_code, const char * message)
{
    Result error;

    error.status = ERROR;

    error.type = error_type;

    if (error.type == ERROR_TYPE_DATABASE)
    {
        error.code.database = error_code;
    } 
    else 
    {
        fprintf(stderr, "\n THE ERROR TYPE YOU USED DOES NOT EXIST IN THE ERROR TYPE ENUM. \n");
        fflush(stderr);
        abort();   
    }

    snprintf(error.message, sizeof(error.message), "%s", message);

    return error;
}

Result create_success()
{
    Result success;

    success.status = SUCCESS;

    return success;
}