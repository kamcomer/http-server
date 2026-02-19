#ifndef REQUEST_H
#define REQUEST_H

#include "http_types.h"

typedef struct
{
  RequestMethod method;
  Protocol protocol;
  ConnectionType type;
  char *user_agent;
  char *platform;
  char *accept;
  char *referer;
  char *accept_encoding;
  char *accept_language;
  Uri uri;
  int error;
} Request;

int header_parse_protocol(Protocol *protocol, char *protocol_str);
int header_parse_method(RequestMethod *method, char *method_str);
int header_parse_protocol_type(Protocol *protocol, char *protocol_str);
int header_parse_protocol_version(Protocol *protocol, char *version_str);

char *read_request(int client_fd);
int parse_request(Request *request, char *header_buff);
int process_request(int client_fd, Request *request);

#endif
