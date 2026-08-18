# include "result.h"

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

    json_clean(format, args, error.message);
    
    return error;
}

int json_clean(const char * format, va_list args, char dst[MAX_ERROR_MESSAGE_LENGTH])
{

    dst[0] = 'h';
    dst[1] = 'i';
    /**/
    char temp[MAX_ERROR_MESSAGE_LENGTH];
    int overfilled = FALSE;

    int written = vsnprintf(temp, MAX_ERROR_MESSAGE_LENGTH, format, args);

    snprintf(dst, MAX_ERROR_MESSAGE_LENGTH, "%s", temp);

    if (written >= MAX_ERROR_MESSAGE_LENGTH)
        overfilled = TRUE;
    
    int padding = 0;
    for (int i = 0; temp[i]!='\0'; i++)
    {
        padding+=(temp[i]=='\"'||temp[i]=='\n');
    }

    if (padding+written >= MAX_ERROR_MESSAGE_LENGTH)
        overfilled = TRUE;

    
    if (overfilled == TRUE)
    {
        printf("HERE");
        strncpy(dst, "WARNING! ERROR MESSAGE BUFFER OVERFILLED! ", MAX_ERROR_MESSAGE_LENGTH);
        overfilled = 42;
    }

    for (int i = 0, j = overfilled; i < MAX_ERROR_MESSAGE_LENGTH && temp[i+j]; i++)
    {
        switch (temp[i]) {
            case '\"':
                dst[i+j] = '\\';
                dst[i+j+1] = '\"';
                j+=1;
                break;
            case '\n':
                dst[i+j] = '\\';
                dst[i+j+1] = 'n';
                dst[i+j+2] = ' ';
                j+=2;
                break;
            default:
                dst[i+j] = temp[i];
                break;
        }
    }

    return 1;
}


Result create_success()
{
    Result success;

    success.status = SUCCESS;

    return success;
}