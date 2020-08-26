#ifndef __hash__
#define __hash__
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "bloom.h"

extern unsigned int seed;//header


unsigned int PJWHash(const void *_str) ;
uint32_t murmur3_32(const void *_str) ;
unsigned int sdbm(const void *_str) ;

#endif
