#ifndef HASHSTRUCTURE_H
#define HASHSTRUCTURE_H
#include "treestructure.h"


typedef struct Pairs {
    char* entry;
    TreeRoot *rootptr;
} Pairs;

typedef struct Bucket {
    Pairs **data;
    int entry_num;
} Bucket;

// A pointer to a dynamic array with pointers to lists of
// buckets
typedef struct Hashtable {
    ll **array;//array of heads to lists
    int hashtable_size;
    int bucket_size;
} Hashtable;


Hashtable *initialize_hashtable(int num_of_entries, int bucket_size);
//void print_hashtable(Hashtable *h_table);
void print_hashtable(Hashtable *h_table);
void *insert_entry_to_ht(Hashtable **h_table, int position, char *data ,void * elem, int (*compare_dates)(struct date,struct date));
Bucket *initialize_bucket(int size);
Hashtable *initialize_hashtable(int hashtable_size, int bucket_size);
unsigned long sdbm(const char *str);

void print_hashtable2(Hashtable *h_table,struct date date_1,struct date date_2,int (*compare_dates)(struct date,struct date));
int print_hash_country(Hashtable *h_table,char *disease,int ps,struct date date_1,struct date date_2,int (*compare_dates)(struct date,struct date),char *answer);
int print_hash_country2(Hashtable *h_table,char *disease,char *country,int ps,struct date date_1,struct date date_2,int (*compare_dates)(struct date,struct date),char *answer);

void print_hashtable3(Hashtable *h_table);
void print_hashtable4(Hashtable *h_table,char *disease,int ps);

void print_exitdate(Hashtable *h_table,char *recordid,char *exitdate);
void print_hs(Hashtable *h_table);
void delete_ht(Hashtable **ht);

void *search_hashtable2(Hashtable **ht, char *ids,MyRecord ***ptr,int **size);
void *search_hashtable3(Hashtable **hc,char *disease, char *d1,char *d2,char *answer);
void *search_hashtable4(Hashtable **hc,char *disease, char *d1,char *d2,char *answer,char * country);
void *search_hashtable5(Hashtable **hc,char *disease, char *d1,char *d2,char *answer);
void *search_hashtable6(Hashtable **hc,char *disease, char *d1,char *d2,char *answer,char * country);

#endif
