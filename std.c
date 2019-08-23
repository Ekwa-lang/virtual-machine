
#include "ekwa.h"
/*
void *
memset(void *, int, size_t);

void *
memcpy(void *, const void *, size_t);

int
memcmp(const void *, const void *, size_t);

size_t
strlen(const char *);

char *
strcpy(char *, const char *);

char *
strncpy(char *, const char *, size_t);
*/

void *
memset(void *dest, int val, size_t len)
{
	unsigned char *ptr = dest;

	while (len-- > 0) {
		*ptr++ = val;
	}

	return dest;
}

void *
memcpy(void *dest, const void *src, size_t len)
{
	const char *s = src;
	char *d = dest;

	while (len--) {
		*d++ = *s++;
	}

	return dest;
}

int
memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *p1 = s1, *p2 = s2;

	while(n--)
		if(*p1 != *p2) {
			return *p1 - *p2;
		}
		else {
			p1++;
			p2++;
		}

	return 0;
}

size_t
strlen(const char *src)
{
	const char *s = src;
	size_t len = 0;

	while (*s++ != 0x00) {
		len++;
	}

	return len;
}

char *
strcpy(char *dest, const char *src)
{
	return memcpy(dest, src, strlen(src) + 1);
}

char *
strncpy(char *dest, const char *src, size_t len)
{
	char *r = memcpy(dest, src, len);
	dest[len] = 0x00;

	return r;
}