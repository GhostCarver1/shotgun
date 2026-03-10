// server.c
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUF 8192


static void send_404(int fd) {
    const char *r =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/plain\r\n"
        "Connection: close\r\n"
        "\r\n"
        "404\n";
    send(fd, r, strlen(r), 0);
}

void send_file(int client_fd, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("file open");
        return;
    }

    printf("Sending file: %s\n", filename);

    char header[] =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n\r\n";

    send(client_fd, header, strlen(header), 0);

    char buffer[1024];
    size_t bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(client_fd, buffer, bytes, 0);
    }

    fclose(file);
}

static void send_text(int fd, const char *text) {
    char hdr[256];
    int n = snprintf(hdr, sizeof(hdr),
                     "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/plain; charset=utf-8\r\n"
                     "Content-Length: %zu\r\n"
                     "Connection: close\r\n"
                     "\r\n",
                     strlen(text));
    send(fd, hdr, (size_t)n, 0);
    send(fd, text, strlen(text), 0);
}

static const char *find_header(const char *req, const char *name) {
    // very simple header search; good enough for demo
    static char needle[128];
    snprintf(needle, sizeof(needle), "\r\n%s:", name);
    const char *p = strstr(req, needle);
    return p ? p + 2 : NULL; // points at "Name:"
}

int main(void) {

    char ** player_list = NULL;
    
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); return 1; }

    int yes = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1 only

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }
    if (listen(s, 16) < 0) { perror("listen"); return 1; }

    printf("Open http://localhost:%d\n", PORT);
    fflush(stdout);

    for (;;) {
        int c = accept(s, NULL, NULL);
        if (c < 0) { perror("accept"); continue; }

        char req[BUF];
        int r = recv(c, req, sizeof(req) - 1, 0);
        if (r <= 0) { close(c); continue; }
        req[r] = '\0';

        // Parse first line: METHOD PATH HTTP/...
        char method[8], path[256];
        if (sscanf(req, "%7s %255s", method, path) != 2) {
            close(c);
            continue;
        }

        // Find body start (for POST)
        const char *body = strstr(req, "\r\n\r\n");

        size_t body_len = 0;
        if (body) {
            body += 4;
            body_len = (size_t)(req + r - body);
        }
        if (strcmp(method, "POST") == 0 && strcmp(path, "/create_player") == 0) {
             
            char *body = strstr(req, "\r\n\r\n");
            if (!body) {
                send_404(c);
                return 1;
            }

            body += 4;

            char player_name[64] = {0};
            player_name[0] = '\0';

            char *name_start = strstr(body, "\"name\":\"");
            if (name_start) {
                name_start += 8;

                char *name_end = strchr(name_start, '"');
                if (name_end) {
                    size_t len = name_end - name_start;
                    strncpy(player_name, name_start, len);
                    player_name[len] = '\0';
                }
            }

            printf("Creating player: %s\n", player_name);
            fflush(stdout);
            printf("Received body: %.*s\n", (int)body_len, body);
            char response[128];
            snprintf(response, sizeof(response), "Player created: %s", player_name);
            close(c);
            player_list = realloc(player_list, sizeof(char*) * (1 + (player_list ? sizeof(player_list) / sizeof(char*) : 0)));
            player_list[0] = strdup(player_name);
            send_text(c, response);
            continue;
        }
        if (strcmp(method, "GET") == 0 && strcmp(path, "/players") == 0) {
            // For demo, return a static list of players in JSON format
            const char *players_json = "{\"players\":[\"%s\",\"Bob\"]}";
            char response[256];
            snprintf(response, sizeof(response), players_json, player_list && player_list[0] ? player_list[0] : "Alice");
            send_text(c, response);
            close(c);
            continue;
        }
        if (strcmp(method, "GET") == 0 && strcmp(path, "/") == 0) {
            send_file(c, "index.html");
        } else if (strcmp(method, "POST") == 0 && strcmp(path, "/echo") == 0) {
            // Echo back what was sent
            // Note: For a real server you'd honor Content-Length and read more if needed.
            char reply[1024];
            size_t ncopy = body_len < sizeof(reply) - 1 ? body_len : sizeof(reply) - 1;
            memcpy(reply, body, ncopy);
            reply[ncopy] = '\0';

            char out[1200];
            snprintf(out, sizeof(out), "Server got: %s", reply);
            send_text(c, out);
        } else {
            send_404(c);
        }

        close(c);
    }
}