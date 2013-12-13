#include "hash.h"

HASH Hash(const char *key_str)
{
	register unsigned int h;
	register unsigned char *p;

	for(h=0, p = (unsigned char *)key_str; *p ; p++)
	{
		h = 31 * h + *p;
	}

	return (h % MAX_SS_NUM);
}