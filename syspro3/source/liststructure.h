#ifndef LISTSTRUCTURE_H
#define LISTSTRUCTURE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
	char*	age;
	date dt;
}MyRecord;

typedef struct linked_list {
    void *data;
    struct linked_list *next;
} ll_node;

// A pointer to the head of the list
typedef struct linked_list_info {
    ll_node *start;
	//int bucket_num;
} ll;

// Allocate memory for a new list node and return it
ll *initialize_list();

// Insert new node at the front and return the newly added node
ll_node* add_ll_node(ll **list_info, void *data, int size_of_structure);

#endif
