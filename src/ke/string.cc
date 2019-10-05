#include <string.h>

size_t strlen(const char* str) {
	size_t len =0;
	while (str[len])
		len++;
	return len;
}

size_t strcmp(const char* s1, const char* s2) {
	while (*s1 == *s2) {
		if (*s1 == '\0') {
			return 0x0;
		}

		s1++;
		s2++;
	}

	return *s1 - *s2;
}

size_t strcmpl(const char* s1, const char* s2, size_t d) {
	for (int i = 0; i < d; i++) {
		if (*s1 != *s2) {
			return 1;
		}

		if (*s1 == '\0' || *s2 == '\0') return 2;

		s1++;
		s2++;
	}

	return 0;
}

bool islower(char c) {
	return (c <= 122 && c >= 97);
}

bool isupper(char c) {
	return (c <= 90 && c >= 65);
}

// In the ascii table, is the character a letter?
bool isletter(char c) {
	return isupper(c) || islower(c);
}

// Set a character to being upper case,
// the ('a' - 'A') means the difference between the upper case and lower case.
char toupper(char c) {
	if (isletter(c)) {
		return c - ('a' - 'A');
	} else {
		return c;
	}
}