#ifndef RESPONSE_H
#define RESPONSE_H
#include "request.h"

int process_response(int client_fd, Request request);
int send_error_response(int client_fd, int status_code, const char *status_text, const char *message);
char *build_header(Request request, char *content_type, long content_size);
char *load_uri(Request request, long *content_size);
#endif