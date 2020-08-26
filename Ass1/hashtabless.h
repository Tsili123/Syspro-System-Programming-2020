#ifndef HASHTABLE_H
#define HASHTABLE_H
 
#include <stdlib.h>
#include <stdio.h>
#include "parse.h"
/*
typedef struct date {
   int month;
   int day;
   int year;
}date;

typedef struct MyRecord{
	char*  	recordID;
	char*	patientFirstName;
	char*	patientLastName;
	char*	diseaseID;
	char*	country;
	char*	entrydate;
	char*	exitdate;
	date dt;
}MyRecord;*/

typedef struct pairs{
	char* item;//pointer 8 bytes
	//TreeNode* treeptr;//pointer 8 bytes
}pairs;
 
typedef struct hashnode {
    pairs* data;//array of pointers based on the size of the bucket
    struct hashnode * next;//pointer -- 8 bytes
    int counter;//index to the availiable place of the pairs array
}hashnode;

typedef int (*hashtable_cmp)(const void*, const void*);
typedef unsigned long (*hashtable_hashfunc)(const void*);
//typedef void (*hashtable_forfn)(void *);
//typedef void (*hashtable_forfn2)(void *, void *);
 
typedef struct hashtable {
    unsigned int size;//size of hashtable
    unsigned int count;//number of elements in hash
    unsigned int bucket_size;
    hashtable_hashfunc hash;//hash function
    hashtable_cmp compare;//compare function
    hashnode **buckets;//array of lists/buckets(2d) .Malloc one dimension to create pointers for the linked list.Malloc the other dimension to add to the list.
    
}hashtable;
 

static unsigned long sdbm(const char *str);
hashnode* create_hashnode(unsigned int pair_num);
hashtable* create_hashtable(unsigned int pair_num,unsigned int size, hashtable_cmp compare);
void add_to_bucket(hashnode *hashnode,MyRecord *data);
/*
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
void hashtable_set_hashfn(hashtable * table, hashtable_hashfn hash);*/
 
#endif /* HASHTABLE_H */
