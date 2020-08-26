#ifndef HASHTABLE_H
#define HASHTABLE_H
 
#include <stdlib.h>
#include <stdio.h>
 
struct hashnode {
    void * data;
    struct hashnode * next;
};
typedef struct hashnode hashnode;
 
typedef int (*hashtable_cmpfn)(const void*, const void*);
typedef unsigned long (*hashtable_hashfn)(const void*);
typedef void (*hashtable_forfn)(void *);
typedef void (*hashtable_forfn2)(void *, void *);
 
struct hashtable {
    unsigned int size;
    hashtable_hashfn hash;
    hashtable_cmpfn compare;
    hashnode **buckets;
    size_t count;
};
typedef struct hashtable hashtable;
 
hashtable *hashtable_create(size_t size, hashtable_cmpfn compare);
void hashtable_empty(hashtable * table);
void hashtable_delete(hashtable * table);
void *hashtable_add(hashtable * table, void * data);
void *hashtable_find(const hashtable * table, const void * data);
void *hashtable_remove(hashtable * table, const void * data);
float hashtable_get_load_factor(const hashtable * table);
unsigned int hashtable_get_count(const hashtable * table);
unsigned int hashtable_find_count(const hashtable *table);
void hashtable_for_each(const hashtable * table, hashtable_forfn fun);
void hashtable_for_each2(const hashtable * table, hashtable_forfn2 fun, void *data);
void hashtable_set_hashfn(hashtable * table, hashtable_hashfn hash);
 
#endif /* HASHTABLE_H */
