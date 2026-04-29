#include "server.h"
#include "request.h"
#include "response.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define DEFAULT_BACKLOG 3
#define KEEP_ALIVE_TIMEOUT 5
#define MAX_KEEP_ALIVE_REQUESTS 10

static volatile sig_atomic_t should_shutdown = 0;

static void handle_shutdown(int sig) {
  (void)sig;
  should_shutdown = 1;
}

static int init_server_socket(int port) {
  struct sockaddr_in socket_address;
  socklen_t address_length = sizeof(socket_address);
  int opt = 1;

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    perror("Failed to create socket");
    return -1;
  }

  // Set SO_REUSEADDR option
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("Failed to set SO_REUSEADDR");
    close(socket_fd);
    return -1;
  }

  // Set SO_REUSEPORT option
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
    perror("Failed to set SO_REUSEPORT");
    close(socket_fd);
    return -1;
  }

  socket_address.sin_family = AF_INET;
  socket_address.sin_addr.s_addr = INADDR_ANY;
  socket_address.sin_port = htons(port);

  if (bind(socket_fd, (struct sockaddr*)&socket_address, address_length) < 0) {
    perror("Failed to bind socket to address");
    close(socket_fd);
    return -1;
  }

  return socket_fd;
}

int server_init(ServerContext* ctx, int port) {
  ctx->port = port;
  ctx->server_fd = init_server_socket(port);

  if (ctx->server_fd < 0) {
    return -1;
  }

  if (listen(ctx->server_fd, DEFAULT_BACKLOG) < 0) {
    perror("Socket failed to listen for incoming clients");
    close(ctx->server_fd);
    return -1;
  }

  // Set up signal handler
  signal(SIGINT, handle_shutdown);

  printf("Server listening on port %d\n", port);
  return 0;
}

int server_accept(ServerContext* ctx) {
  struct sockaddr_in client_socket_addr;
  socklen_t addr_len = sizeof(client_socket_addr);

  int client_fd = accept(ctx->server_fd, (struct sockaddr*)&client_socket_addr, &addr_len);
  if (client_fd < 0) {
    perror("Failed to accept new client");
    return -1;
  }

  printf("Client connected: %s:%d\n", inet_ntoa(client_socket_addr.sin_addr),
         ntohs(client_socket_addr.sin_port));

  return client_fd;
}

void server_shutdown(ServerContext* ctx) {
  printf("\nShutting down the server...\n");
  close(ctx->server_fd);
}

static int handle_single_request(int client_fd) {
  Request request;

  if (process_request(client_fd, &request) < 0) {
    send_error_response(client_fd, 400, "Bad Request", "Invalid request format");
    return -1;
  }

  int keep_alive = 0;
  if (request.headers.connection != NULL) {
    if (strcasecmp(request.headers.connection, "keep-alive") == 0) {
      keep_alive = 1;
    }
  }

  process_response(client_fd, request.uri.path, request.protocol.major_version,
                   request.protocol.minor_version, request.method);

  free_request(&request);
  return keep_alive;
}

static void handle_connection(int client_fd) {
  struct timeval timeout;
  timeout.tv_sec = KEEP_ALIVE_TIMEOUT;
  timeout.tv_usec = 0;
  setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

  int requests_served = 0;

  while (requests_served < MAX_KEEP_ALIVE_REQUESTS) {
    int keep_alive = handle_single_request(client_fd);

    if (keep_alive <= 0) {
      break;
    }

    requests_served++;
  }

  close(client_fd);
}

int server_run(ServerContext* ctx) {
  while (!should_shutdown) {
    int client_fd = server_accept(ctx);
    if (client_fd < 0) {
      continue;
    }

    pid_t pid = fork();
    if (pid < 0) {
      perror("Fork failed");
      close(client_fd);
      continue;
    }

    if (pid == 0) {
      close(ctx->server_fd);
      handle_connection(client_fd);
      exit(0);
    }

    close(client_fd);
  }

  server_shutdown(ctx);
  return 0;
}
