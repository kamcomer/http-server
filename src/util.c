#include "util.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void remove_whitespace(char *str)
{
  int len = strlen(str);
  char new_str[len];

  int j = 0;
  for (int i = 0; i < len; i++)
  {
    if (!isspace(str[i]))
    {
      new_str[j++] = str[i];
      str[i] = '\0';
    }
  }

  strcpy(str, new_str);
}

void str_to_upper(char *str)
{
  for (int i = 0; str[i]; i++)
  {
    str[i] = toupper(str[i]);
  }
}

typedef struct {
  const char *extension;
  const char *mime_type;
} MimeTypeEntry;

static const MimeTypeEntry mime_types[] = {
  { "html", "text/html" },
  { "htm", "text/html" },
  { "css", "text/css" },
  { "js", "application/javascript" },
  { "json", "application/json" },
  { "xml", "application/xml" },
  { "txt", "text/plain" },
  { "png", "image/png" },
  { "jpg", "image/jpeg" },
  { "jpeg", "image/jpeg" },
  { "gif", "image/gif" },
  { "svg", "image/svg+xml" },
  { "ico", "image/x-icon" },
  { "webp", "image/webp" },
  { "pdf", "application/pdf" },
  { "zip", "application/zip" },
  { "woff", "font/woff" },
  { "woff2", "font/woff2" },
  { "ttf", "font/ttf" },
  { "otf", "font/otf" },
  { "mp4", "video/mp4" },
  { "webm", "video/webm" },
  { "mp3", "audio/mpeg" },
  { "wav", "audio/wav" },
  { "ogg", "audio/ogg" },
};

static const int num_mime_types = sizeof(mime_types) / sizeof(mime_types[0]);

const char *get_mime_type(const char *path)
{
  if (path == NULL)
  {
    return "application/octet-stream";
  }

  const char *last_dot = strrchr(path, '.');
  if (last_dot == NULL)
  {
    return "application/octet-stream";
  }

  last_dot++;

  for (int i = 0; i < num_mime_types; i++)
  {
    if (strcasecmp(last_dot, mime_types[i].extension) == 0)
    {
      return mime_types[i].mime_type;
    }
  }

  return "application/octet-stream";
}

char *url_decode(const char *encoded)
{
  if (encoded == NULL)
  {
    return NULL;
  }

  size_t len = strlen(encoded);
  char *decoded = malloc(len + 1);
  if (decoded == NULL)
  {
    return NULL;
  }

  size_t j = 0;
  for (size_t i = 0; i < len; i++)
  {
    if (encoded[i] == '%' && i + 2 < len)
    {
      char hex[3] = { encoded[i + 1], encoded[i + 2], '\0' };
      char *endptr;
      int value = (int)strtol(hex, &endptr, 16);
      if (*endptr == '\0' && value >= 0 && value <= 255)
      {
        decoded[j++] = (char)value;
        i += 2;
        continue;
      }
    }
    else if (encoded[i] == '+')
    {
      decoded[j++] = ' ';
    }
    else
    {
      decoded[j++] = encoded[i];
    }
  }

  decoded[j] = '\0';
  return decoded;
}
