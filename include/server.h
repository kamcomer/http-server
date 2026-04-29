#ifndef SERVER_H
#define SERVER_H

typedef struct {
  int server_fd;
  int port;
} ServerContext;

int server_init(ServerContext* ctx, int port);
int server_accept(ServerContext* ctx);
void server_shutdown(ServerContext* ctx);
int server_run(ServerContext* ctx);

#endif
