#ifndef RESULT_H
#define RESULT_H

# include "../constants.h"

typedef enum
{
    SUCCESS,
    ERROR
} Status;

typedef enum
{
    ERROR_TYPE_DATABASE,
} ErrorType;

typedef enum
{
    ERROR_CODE_DATABASE_QUERY_INVALID,
    ERROR_CODE_DATABASE_CONNECTION_FAILED,
    ERROR_CODE_DATABASE_QUERY_EMPTY
} DatabaseError;


typedef enum
{
    REQUEST_BAD,
} RequestError;

typedef struct
{
    Status status;

    ErrorType type;

    union 
    {
        DatabaseError database;
        RequestError request;
    }   code;

    char message[MAX_ERROR_MESSAGE_LENGTH];

} Result;

Result create_error(ErrorType error_type, int error_code, const char * message);

Result create_success();

#endif