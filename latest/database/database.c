#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <postgresql/libpq-fe.h>
#include <sodium.h>



PGconn *db_connect(void)
{
    char conninfo[256];
    
    snprintf(conninfo, sizeof(conninfo), 
    "host=%s port=%s dbname=%s user=%s password=%s", getenv("DB_HOST"),getenv("DB_PORT"),getenv("DB_NAME"),getenv("DB_USER"),getenv("DB_PASSWORD"));

    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Database connection failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return NULL;
    }

    return conn;
}

void db_disconnect(PGconn *conn)
{
    if (conn) {
        PQfinish(conn);
    }
}

int db_get_player_info(PGconn * conn, const char * user_email, char * hash_output, char * user_id_output, char * user_name_output, size_t hash_output_size, size_t user_id_output_size, size_t user_name_output_size)
{
    const char *sql = 
        "SELECT user_hash, player_id, user_name from players where user_email = ($1);";
    
    const char *params[1] = {
        user_email
    };

    PGresult *res = PQexecParams(
        conn,sql,1,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "get player info failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return 0;
    }

    if (PQntuples(res) == 0) {
        fprintf(stderr, "No player found with email: %s\n", user_email);
        PQclear(res);
        return 0;
    }

    strncpy(hash_output, PQgetvalue(res, 0, 0), hash_output_size - 1);
    hash_output[hash_output_size - 1] = '\0';

    strncpy(user_id_output, PQgetvalue(res, 0, 1), user_id_output_size - 1);
    user_id_output[user_id_output_size - 1] = '\0';

    strncpy(user_name_output, PQgetvalue(res, 0, 2), user_name_output_size - 1);
    user_name_output[user_name_output_size - 1] = '\0';

    PQclear(res);
    return 1;
}

int db_get_player_id(PGconn *conn, const char *user_name, const char * user_email)
{
   const char *sql = 
        "SELECT player_id from players where "
        "user_name == ($1) AND user_email == ($2) ;";
    
    const char *params[2] = {
        user_name,
        user_email
    };

    PGresult *res = PQexecParams(
        conn,sql,2,NULL,params,NULL,NULL,0
    );

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "get player failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return -1;
    }

    int player_id = atoi(PQgetvalue(res, 0, 0));

    PQclear(res);
    return player_id;
}