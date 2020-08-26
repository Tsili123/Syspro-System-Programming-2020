#include "hashtb.h"
 
hashnode * hashnode_create(void * data)
{
    hashnode * node = malloc(sizeof(hashnode));
    if (node) {
        node->data = data;
        node->next = NULL;
    }
    return node;
}
 
void hashnode_delete(hashnode * node)
{
    free(node);
}
 
static unsigned long sdbm(const char *str)
{
    unsigned long hash = 0;
    int c;
 
    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;
 
    return hash;
}
 
hashtable * hashtable_create(size_t size, hashtable_cmpfn compare)
{
    hashtable * table = malloc(sizeof (hashtable));
    if (table) {
        table->size = size;
        table->hash = (hashtable_hashfn)sdbm;
        table->compare = compare;
        table->count = 0;
        table->buckets = malloc(size * sizeof(hashnode *));
        if (table->buckets) {
            unsigned int b;
            for (b = 0; b < size; b++) {
                table->buckets[b] = NULL;
            }
        }
        else {
            free(table);
            table = NULL;
        }
    }
    return table;
}
 
void hashtable_empty(hashtable * table)
{
    unsigned int i;
    hashnode * temp;
    for (i = 0; i < table->size; i++) {
        hashnode * current = table->buckets[i];
        while (current != NULL) {
            temp = current->next;
            hashnode_delete(current);
            current = temp;
        }
        table->buckets[i] = NULL;
    }
    table->count = 0;
}
 
void hashtable_delete(hashtable * table)
{
    if (table) {
        hashtable_empty(table);
        free(table->buckets);
        free(table);
    }
}
 
void * hashtable_add(hashtable * table, void * data)
{
    const unsigned int bucket = table->hash(data) % table->size;
    void * found = NULL;
 
    if (table->buckets[bucket] == NULL) {
        /* An empty bucket */
        table->buckets[bucket] = hashnode_create(data);
    }
    else {
        unsigned int added = 0;
        hashnode * current, * previous = NULL;
        for (current = table->buckets[bucket]; current != NULL && !found && !added; current = current->next) {
            const int result = table->compare(current->data, data);
            if (result == 0) {
                /* Changing an existing entry */
                found = current->data;
                current->data = data;
            }
            else if (result > 0) {
                /* Add before current */
                hashnode * node = hashnode_create(data);
                node->next = current;
                if (previous == NULL) {
                    /* Adding at the front */
                    table->buckets[bucket] = node;
                }
                else {
                    previous->next = node;
                }
                added = 1;
            }
            previous = current;
        }
        if (!found && !added && current == NULL) {
            /* Adding at the end */
            previous->next = hashnode_create(data);
        }
    }
    if (found == NULL) {
        table->count++;
    }
 
    return found;
}
 
void * hashtable_find(const hashtable * table, const void * data)
{
    hashnode * current;
    const unsigned int bucket = table->hash(data) % table->size;
    void * found = NULL;
    unsigned int passed = 0;
    for (current = table->buckets[bucket]; current != NULL && !found && !passed; current = current->next) {
        const int result = table->compare(current->data, data);
        if (result == 0) {
            found = current->data;
        }
        else if (result > 0) {
            passed = 1;
        }
    }
    return found;
}
 
void * hashtable_remove(hashtable * table, const void * data)
{
    hashnode * current, * previous = NULL;
    const unsigned int bucket = table->hash(data) % table->size;
    void * found = NULL;
    unsigned int passed = 0;
 
    current = table->buckets[bucket];
    while (current != NULL && !found && !passed) {
        const int result = table->compare(current->data, data);
        if (result == 0) {
            found = current->data;
            if (previous == NULL) {
                /* Removing the first entry */
                table->buckets[bucket] = current->next;
            }
            else {
                previous->next = current->next;
            }
            hashnode_delete(current);
            table->count--;
        }
        else if (result > 0) {
            passed = 1;
        }
        else {
            previous = current;
            current = current->next;
        }
    }
    return found;
}
 
 
float hashtable_get_load_factor(const hashtable * table)
{
    unsigned int touched = 0;
    float loadfactor;
    unsigned int b;
    for (b = 0; b < table->size; b++) {
        if (table->buckets[b] != NULL) {
            touched++;
        }
    }
    loadfactor = (float)touched / (float)table->size;
    return loadfactor;
}
 
unsigned int hashtable_get_count(const hashtable * table)
{
    return table->count;
}
 
unsigned int hashtable_find_count(const hashtable *table)
{
    unsigned int b;
    const hashnode *node;
    unsigned int count = 0;
    for (b = 0; b < table->size; b++) {
        for (node = table->buckets[b]; node != NULL; node = node->next) {
            count++;
        }
    }
    return count;
}
 
void hashtable_for_each(const hashtable * table, hashtable_forfn fun)
{
    unsigned int b;
 
    for (b = 0; b < table->size; b++) {
        const hashnode *node;
        for (node = table->buckets[b]; node != NULL; node = node->next) {
            fun(node->data);
        }
    }
}
 
 
void hashtable_for_each2(const hashtable * table, hashtable_forfn2 fun, void *data)
{
    unsigned int b;
 
    for (b = 0; b < table->size; b++) {
        const hashnode *node;
        for (node = table->buckets[b]; node != NULL; node = node->next) {
            fun(node->data, data);
        }
    }
}
 
void hashtable_set_hashfn(hashtable * table, hashtable_hashfn hash)
{
    table->hash = hash;
}
