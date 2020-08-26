#include <stdio.h>
#include <string.h>
 
#include "hashtb.h"
 
int main(void)
{
    hashtable * table;
    const char * result;
    unsigned int e;
    char * elements[] = {"A", "B", "C", "D", "E", "F"};
    const unsigned int n = sizeof(elements) / sizeof(const char*);
 
    table = hashtable_create(3, (hashtable_cmpfn)strcmp);
    for (e = 0; e < n; e++) {
        hashtable_add(table, elements[e]);
    }
    hashtable_for_each(table, (hashtable_forfn)puts);
    for (e = 0; e < n; e++) {
        result = hashtable_find(table, elements[e]);
        if (result) {
            printf("Found: %s\n", result);
        }
        else {
            printf("Couldn't find %s\n", elements[e]);
        }
    }
    printf("The load factor is %.2f\n", hashtable_get_load_factor(table));
    for (e = 0; e < n; e++) {
        result = hashtable_remove(table, elements[e]);
        if (result) {
            printf("Removed: %s\n", result);
        }
        else {
            printf("Couldn't remove %s\n", elements[e]);
        }
    }
    hashtable_delete(table);
 
    return 0;
}
