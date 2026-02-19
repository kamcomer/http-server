#ifndef RESPONSE_H
#define RESPONSE_H

#include "http_types.h"

int process_response(int client_fd, const char *path, int protocol_major, int protocol_minor);
int send_error_response(int client_fd, int status_code, const char *status_text, const char *message);
char *build_header(int status_code, const char *status_text, int protocol_major, int protocol_minor,
                   const char *content_type, long content_size);
char *load_uri(const char *path, long *content_size);

#endif
