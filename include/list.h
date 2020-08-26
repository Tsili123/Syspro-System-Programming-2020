//linked list for client
#ifndef LIST_HEADER
#define LIST_HEADER

#include "../include/defines.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>


typedef struct Tuple{
    unsigned int num_port;
    struct in_addr ip_address;
}Tuple;

//A list node
typedef struct Listnode{
    Tuple tuple;
    struct Listnode *next;
}Listnode;

//A pointer to the head of the list
typedef struct List{
    Listnode *headlist;
    int size;
}List;

// Compare two tuples and if they are the same return 0,else return 1
int compare_tuples(Tuple tuplefirst, Tuple tuplesecond);

// Allocate memory for new list
List *initialize_list(void)

// Insert new node at the beginning or if exists return null
Listnode *add_listnode(List *list, Tuple data);

// Search for a node and return it, return NULL if it doesn't exist
Listnode *search_listnode(List *list, Tuple data);

// Print all tuples inside list
void print_list(List *list)

// Release memory of node structure
// returns 1 if it doesn't exist, else 0
int delete_listnode(List *list, Tuple data)

// Release memory
void delete_list(List *list);
#endif