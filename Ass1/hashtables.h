#ifndef ASSIGNMENT_1_HASHTABLE_H
#define ASSIGNMENT_1_HASHTABLE_H
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
//#include <list.h>

//an array of buckets(2-dimensions)
typedef void ** Bucket;

typedef struct Hashtable
{
    Bucket buckets;
    size_t size;
    size_t bucket_size;
} Hashtable;

Bucket initialize_bucket(size_t bucket_size);
void insert_to_bucket(Bucket bucket, size_t bucket_size , void* records);
Hashtable* initialize_hashtable(size_t hashtable_size, size_t bucket_size);
size_t hash_sdbm(const void *_str , int max);

#endif 
