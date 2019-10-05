#ifndef STRING_H
#define STRING_H
#include <system.h>

size_t strlen(const char* str);
size_t strcmp(const char* s1, const char* s2);
size_t strcmpl(const char* s1, const char* s2, size_t d);
bool islower(char c);
bool isupper(char c);
bool isletter(char c);
char toupper(char c);

#endif