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
    ERROR_TYPE_JSON,
} ErrorType;

typedef enum
{
    ERROR_CODE_DATABASE_QUERY_INVALID,
    ERROR_CODE_DATABASE_CONNECTION_FAILED,
    ERROR_CODE_DATABASE_QUERY_EMPTY
} DatabaseError;

typedef enum
{
    ERROR_CODE_JSON_KEY_MISSING = -1,
    ERROR_CODE_JSON_EMPTY = -2,
    ERROR_CODE_JSON_MALFORMED = -3
} JsonError;


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
        JsonError json;
    }   code;

    char message[MAX_ERROR_MESSAGE_LENGTH];

} Result;

Result create_error(ErrorType error_type, int error_code, const char * format, ...);

Result create_success();

#endif