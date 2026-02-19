#include "response.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>

char *load_uri(const char *path, long *content_size)
{
  FILE *file;
  file = fopen(path, "rb");
  if (file == NULL)
  {
    perror("Error opening file");
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  *content_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *contents = malloc(*content_size);
  if (contents == NULL)
  {
    perror("Error allocating memory");
    fclose(file);
    return NULL;
  }

  fread(contents, 1, *content_size, file);
  fclose(file);

  return contents;
}

char *build_header(int status_code, const char *status_text, int protocol_major, int protocol_minor, 
                   const char *content_type, long content_size)
{
  char *response_header = malloc(BUFFER_SIZE);
  if (response_header == NULL)
  {
    perror("Error allocating memory");
    return NULL;
  }
  snprintf(response_header, BUFFER_SIZE,
           "HTTP/%d.%d %d %s\r\n"
           "Content-Type: %s\r\n"
           "Content-Length: %ld\r\n"
           "Connection: close\r\n\r\n",
           protocol_major, protocol_minor,
           status_code, status_text,
           content_type,
           content_size);

  return response_header;
}

int send_error_response(int client_fd, int status_code, const char *status_text, const char *message)
{
  char response[BUFFER_SIZE];
  const char *html_template =
      "<!DOCTYPE html>\r\n"
      "<html><head><title>%d %s</title></head>\r\n"
      "<body><h1>%d %s</h1><p>%s</p></body></html>\r\n";

  char html_body[512];
  snprintf(html_body, sizeof(html_body), html_template,
           status_code, status_text, status_code, status_text, message);

  size_t html_len = strlen(html_body);

  snprintf(response, BUFFER_SIZE,
           "HTTP/1.1 %d %s\r\n"
           "Content-Type: text/html\r\n"
           "Content-Length: %zu\r\n"
           "Connection: close\r\n\r\n"
           "%s",
           status_code, status_text, html_len, html_body);

  send(client_fd, response, strlen(response), 0);
  return -1;
}

int process_response(int client_fd, const char *path, int protocol_major, int protocol_minor)
{
  char *decoded_path = url_decode(path);
  if (decoded_path == NULL)
  {
    send_error_response(client_fd, 400, "Bad Request", "Invalid URL encoding.");
    return -1;
  }

  if (!is_path_safe(decoded_path))
  {
    free(decoded_path);
    send_error_response(client_fd, 403, "Forbidden", "Access denied: path traversal not allowed.");
    return -1;
  }

  const char *mime_type = get_mime_type(decoded_path);

  long content_size;
  char *contents = load_uri(decoded_path, &content_size);

  if (contents == NULL)
  {
    free(decoded_path);
    send_error_response(client_fd, 404, "Not Found", "The requested resource was not found.");
    return -1;
  }

  printf("Serving file: %s (type: %s)\n", decoded_path, mime_type);

  char *header = build_header(200, "OK", protocol_major, protocol_minor, mime_type, content_size);
  if (header == NULL)
  {
    free(contents);
    free(decoded_path);
    send_error_response(client_fd, 500, "Internal Server Error", "Failed to build response header.");
    return -1;
  }

  send(client_fd, header, strlen(header), 0);
  send(client_fd, contents, content_size, 0);

  free(contents);
  free(decoded_path);
  free(header);
  return 0;
}
