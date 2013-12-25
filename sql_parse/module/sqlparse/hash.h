#ifndef _HASH_H_
#define _HASH_H_

#include <stdio.h>
#include <string.h>
//#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#define HASH_GENE 4
#define OVER_ (65535*65535)
typedef int HASH;
const unsigned long MAX_SS_NUM = 1024;
HASH Hash(const char *key_str);

#endif //_HASH_H_