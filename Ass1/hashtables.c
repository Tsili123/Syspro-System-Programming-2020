#include "hashtable.h"
#include "BST.h"

Bucket initialize_bucket(size_t bucket_size){
	size_t num_entry = bucket_size / sizeof(void*),i;
    Bucket bucket = malloc(bucket_size);
    
    for(i = 0; i < num_entry; i++){
        bucket[i] = NULL;
    }
    
    bucket[num_entry - 2] = (void*)0;
    return bucket;//returns 1-d array 
}

void insert_to_bucket(Bucket bucket, size_t bucket_size , void* records){
	size_t num_entry = bucket_size / sizeof(void*);
    size_t counter = (size_t)bucket[num_entry - 2];
	Bucket new_bucket;

    if(counter == num_entry - 2){
        new_bucket = initialize_bucket(bucket_size);
        bucket[num_entry - 1] = (void*)new_bucket;
        insert_to_bucket(new_bucket, bucket_size , records);
    }
    else{
        bucket[counter] = records;
        bucket[num_entry - 2] = (void*)(counter + 1);
    }
}

Hashtable* initialize_hashtable(size_t hashtable_size, size_t bucket_size){
	size_t i;
    Hashtable* hashtable = malloc(sizeof(hashtable));
    hashtable->buckets = malloc(hashtable_size * sizeof(void*));
    
    for(i = 0; i < hashtable_size; i++){
        hashtable->buckets[i] = initialize_bucket(bucket_size);
    }
    
    hashtable->size = hashtable_size;
    hashtable->bucket_size = bucket_size;
    
    return hashtable;
}


//hash function
size_t hash_sdbm(const void *_str , int max) {
  size_t hash = 0;
  const char *str = _str;
  char c;
  while ((c = *str++))
    hash = c + (hash << 6) + (hash << 16) - hash;
  return abs(hash % max);
}



