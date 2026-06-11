#ifndef DATABASE_H
#define DATABASE_H

#include <postgresql/libpq-fe.h>

typedef enum
{
    DB_SUCCESS = 1,
    DB_EMPTY = 0,
    DB_QUERY_INVALID = -1,
} DbQueryStatus;

PGconn *db_connect(void);
void db_disconnect(PGconn *conn);
int db_get_player_info(PGconn * conn, const char * user_email, char * hash_output, char * user_id_output, char * user_name_output, size_t hash_output_size, size_t user_id_output_size, size_t user_name_output_size);
int db_get_player_id(PGconn *conn, const char *user_name, const char * user_email);

#endif