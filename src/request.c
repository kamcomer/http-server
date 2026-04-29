#include "request.h"
#include "util.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int header_parse_method(RequestMethod* method, char* method_str) {
  if (method_str == NULL) {
    fprintf(stderr, "Error: NULL pointer passed as methodString\n");
    return INVALID_REQUEST_HEADER;
  }

  str_to_upper(method_str);

  if (strcmp(method_str, "GET") == 0) {
    *method = METHOD_GET;
  } else if (strcmp(method_str, "HEAD") == 0) {
    *method = METHOD_HEAD;
  } else if (strcmp(method_str, "OPTIONS") == 0) {
    printf("Method Type Not Supported: OPTIONS\n");
    return INVALID_REQUEST_HEADER;
  } else if (strcmp(method_str, "POST") == 0) {
    printf("Method Type Not Supported: POST\n");
    return INVALID_REQUEST_HEADER;
  } else if (strcmp(method_str, "PUT") == 0) {
    printf("Method Type Not Supported: PUT\n");
    return INVALID_REQUEST_HEADER;
  } else if (strcmp(method_str, "DELETE") == 0) {
    printf("Method Type Not Supported: DELETE\n");
    return INVALID_REQUEST_HEADER;
  } else if (strcmp(method_str, "TRACE") == 0) {
    printf("Method Type Not Supported: TRACE\n");
    return INVALID_REQUEST_HEADER;
  } else if (strcmp(method_str, "CONNECT") == 0) {
    printf("Method Type Not Supported: CONNECT\n");
    return INVALID_REQUEST_HEADER;
  } else {
    fprintf(stderr, "Error: Unknown HTTP method: %s\n", method_str);
    return INVALID_REQUEST_HEADER;
  }

  return 0;
}

int header_parse_protocol(Protocol* protocol, char* protocol_str) {
  if (header_parse_protocol_type(protocol, protocol_str) < 0) {
    return INVALID_REQUEST_HEADER;
  }
  if (header_parse_protocol_version(protocol, protocol_str) < 0) {
    return INVALID_REQUEST_HEADER;
  }
  return 0;
}

int header_parse_protocol_type(Protocol* protocol, char* protocol_str) {
  if (protocol_str == NULL) {
    fprintf(stderr, "Error: NULL pointer passed as protocolString\n");
    return INVALID_REQUEST_HEADER;
  }

  str_to_upper(protocol_str);

  char* slash_pos = strstr(protocol_str, "/");
  if (slash_pos == NULL) {
    fprintf(stderr, "Error: Invalid protocol string\n");
    return INVALID_REQUEST_HEADER;
  }
  int offset = slash_pos - protocol_str;
  char* type = malloc(offset + 1);
  if (type == NULL) {
    fprintf(stderr, "Error: unable to allocate memory\n");
    return INVALID_REQUEST_HEADER;
  }

  strncpy(type, protocol_str, offset);
  type[offset] = '\0';

  ProtocolType result;
  if (strcmp(type, "HTTP") == 0) {
    protocol->type = PROTOCOL_HTTP;
  } else {
    return INVALID_REQUEST_HEADER;
  }

  free(type);
  return 0;
}

int header_parse_protocol_version(Protocol* protocol, char* version_str) {
  if (version_str == NULL) {
    fprintf(stderr, "Error: NULL pointer passed as versionString\n");
    return INVALID_REQUEST_HEADER;
  }

  while (*version_str && !isdigit(*version_str))
    version_str++;

  int major_version = INVALID_VERSION_NUMBER, minor_version = INVALID_VERSION_NUMBER;
  if (sscanf(version_str, "%d.%d", &major_version, &minor_version) != 2) {
    fprintf(stderr, "Error: Invalid protocol version format\n");
    return INVALID_REQUEST_HEADER;
  }

  protocol->major_version = major_version;
  if (protocol->major_version < 0) {
    fprintf(stderr, "Error: Invalid protocol major version");
    return INVALID_REQUEST_HEADER;
  }

  protocol->minor_version = minor_version;
  if (protocol->minor_version < 0) {
    fprintf(stderr, "Error: Invalid protocol minor version");
    return INVALID_REQUEST_HEADER;
  }
  return 0;
}

int process_request(int client_fd, Request* request) {
  char* read_buff = read_request(client_fd);
  if (read_buff == NULL) {
    return -1;
  }

  printf("\nClient Request:\n%s\n", read_buff);
  if (parse_request(request, read_buff) < 0) {
    return -1;
  }

  return 0;
}

int parse_request(Request* request, char* buff) {

  if (buff == NULL) {
    fprintf(stderr, "Error: NULL pointer passed as buff");
    return INVALID_REQUEST_HEADER;
  }

  if (strlen(buff) == 0) {
    fprintf(stderr, "Error: Empty string passes as buff");
    return INVALID_REQUEST_HEADER;
  }

  char method[MAX_METHOD_LENGTH];
  char uri[MAX_URI_LENGTH];
  char protocol[MAX_PROTOCOL_LENGTH];
  char buffer_cpy[BUFFER_SIZE];

  strncpy(buffer_cpy, buff, sizeof(buffer_cpy));

  char* line = strtok(buffer_cpy, "\n");

  if (sscanf(line, "%s %s %s", method, uri, protocol) != 3) {
    fprintf(stderr, "Error: Invalid request header format\n");
    return INVALID_REQUEST_HEADER;
  }

  if (header_parse_method(&request->method, method) < 0) {
    return INVALID_REQUEST_HEADER;
  }

  if (header_parse_protocol(&request->protocol, protocol) < 0) {
    return INVALID_REQUEST_HEADER;
  }

  request->uri.path = strdup(uri + 1);

  if (request->uri.path == NULL) {
    fprintf(stderr, "Error: Unable to allocate memory for request URI\n");
    return INVALID_REQUEST_HEADER;
  }

  if (header_parse_headers(&request->headers, buff) < 0) {
    fprintf(stderr, "Error: Failed to parse headers\n");
    free(request->uri.path);
    return INVALID_REQUEST_HEADER;
  }

  return 0;
}

char* read_request(int client_fd) {
  size_t total_size = 0;
  size_t buffer_size = BUFFER_SIZE;
  char* read_buff = malloc(BUFFER_SIZE);
  if (read_buff == NULL) {
    perror("Error allocating memory");
    return NULL;
  }
  while (true) {
    if (total_size >= MAX_REQUEST_SIZE) {
      fprintf(stderr, "Error: Request too large\n");
      free(read_buff);
      return NULL;
    }

    int read_status = read(client_fd, read_buff + total_size, buffer_size - total_size);
    if (read_status < 0) {
      perror("Error reading request");
      free(read_buff);
      return NULL;
    }

    if (read_status == 0) {
      break;
    }

    total_size += read_status;

    if (read_status < (int)(buffer_size - total_size)) {
      break;
    }

    if (total_size >= MAX_REQUEST_SIZE) {
      fprintf(stderr, "Error: Request too large\n");
      free(read_buff);
      return NULL;
    }

    if (total_size == buffer_size) {
      buffer_size += BUFFER_SIZE;
      if (buffer_size > MAX_REQUEST_SIZE) {
        buffer_size = MAX_REQUEST_SIZE + 1;
      }
      char* temp = realloc(read_buff, buffer_size);
      if (temp == NULL) {
        perror("Error reallocating memory");
        free(read_buff);
        return NULL;
      }
      read_buff = temp;
    }
  }
  read_buff[total_size] = '\0';
  return read_buff;
}

static char* trim_whitespace(char* str) {
  while (isspace((unsigned char)*str)) {
    str++;
  }

  if (*str == 0) {
    return str;
  }

  char* end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end)) {
    end--;
  }
  *(end + 1) = '\0';

  return str;
}

int header_parse_headers(HttpHeaders* headers, char* request_body) {
  if (headers == NULL || request_body == NULL) {
    return -1;
  }

  memset(headers, 0, sizeof(HttpHeaders));

  char* body_copy = strdup(request_body);
  if (body_copy == NULL) {
    return -1;
  }

  char* line = strtok(body_copy, "\r\n");
  int line_count = 0;

  while (line != NULL && line_count < MAX_HEADERS) {
    line_count++;

    if (line_count == 1) {
      line = strtok(NULL, "\r\n");
      continue;
    }

    if (strlen(line) == 0) {
      break;
    }

    char* colon = strchr(line, ':');
    if (colon != NULL) {
      *colon = '\0';
      char* name = trim_whitespace(line);
      char* value = trim_whitespace(colon + 1);

      if (strcasecmp(name, "Host") == 0) {
        headers->host = strdup(value);
      } else if (strcasecmp(name, "User-Agent") == 0) {
        headers->user_agent = strdup(value);
      } else if (strcasecmp(name, "Accept") == 0) {
        headers->accept = strdup(value);
      } else if (strcasecmp(name, "Accept-Language") == 0) {
        headers->accept_language = strdup(value);
      } else if (strcasecmp(name, "Accept-Encoding") == 0) {
        headers->accept_encoding = strdup(value);
      } else if (strcasecmp(name, "Connection") == 0) {
        headers->connection = strdup(value);
      } else if (strcasecmp(name, "Content-Type") == 0) {
        headers->content_type = strdup(value);
      } else if (strcasecmp(name, "Content-Length") == 0) {
        headers->content_length = strdup(value);
      } else if (strcasecmp(name, "Referer") == 0) {
        headers->referer = strdup(value);
      } else if (strcasecmp(name, "Cookie") == 0) {
        headers->cookie = strdup(value);
      }
    }

    line = strtok(NULL, "\r\n");
  }

  free(body_copy);
  return 0;
}

void free_request(Request* request) {
  if (request == NULL) {
    return;
  }

  if (request->uri.path != NULL) {
    free(request->uri.path);
  }

  if (request->headers.host != NULL) {
    free(request->headers.host);
  }
  if (request->headers.user_agent != NULL) {
    free(request->headers.user_agent);
  }
  if (request->headers.accept != NULL) {
    free(request->headers.accept);
  }
  if (request->headers.accept_language != NULL) {
    free(request->headers.accept_language);
  }
  if (request->headers.accept_encoding != NULL) {
    free(request->headers.accept_encoding);
  }
  if (request->headers.connection != NULL) {
    free(request->headers.connection);
  }
  if (request->headers.content_type != NULL) {
    free(request->headers.content_type);
  }
  if (request->headers.content_length != NULL) {
    free(request->headers.content_length);
  }
  if (request->headers.referer != NULL) {
    free(request->headers.referer);
  }
  if (request->headers.cookie != NULL) {
    free(request->headers.cookie);
  }
}