#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "bstree.h"


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
void print_hash_country(Hashtable *h_table,char *disease,int ps,struct date date_1,struct date date_2,int (*compare_dates)(struct date,struct date));
void print_hash_country2(Hashtable *h_table,char *disease,char *country,int ps,struct date date_1,struct date date_2,int (*compare_dates)(struct date,struct date));

void print_hashtable3(Hashtable *h_table);
void print_hashtable4(Hashtable *h_table,char *disease,int ps);

void print_exitdate(Hashtable *h_table,char *recordid,char *exitdate);
void print_hs(Hashtable *h_table);
void delete_ht(Hashtable **ht);
#endif
