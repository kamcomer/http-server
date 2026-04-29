#ifndef UTIL_H
#define UTIL_H

void remove_whitespace(char* str);
void str_to_upper(char* str);

const char* get_mime_type(const char* path);
char* url_decode(const char* encoded);

int is_path_safe(const char* path);

#endif
