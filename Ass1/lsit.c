#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>

// Create a new linked list node with a given item:
Node* initialize_node(void* records)
{
    Node* new_node = malloc(sizeof(Node));
    new_node->records = records;
    new_node->next = NULL;
    return new_node;
}

// Create a new linked list and simply set the head and tail to the given node:
LinkedList* initialize_linked_list(Node* node)
{
    linked_list* new_linked_list = malloc(sizeof(LinkedList));
    new_linked_list->head = node;
    new_linked_list->tail = node;
    return newLinkedList;
}

// Append item to a linked list:
void appendToLinkedList(LinkedList* linkedList, Node* node)
{
    if(linkedList->head == NULL)
    {
        linkedList->head = node;
        linkedList->tail = node;
    }
    else
    {
        linkedList->tail->next = node;
        linkedList->tail = node;
    }
}

// Pop a node from the beginning of the list:
Node* popStart(LinkedList* linkedList)
{
    Node* node = linkedList->head;
    linkedList->head = linkedList->head->next;
    if(linkedList->head == NULL)
        linkedList->tail = NULL;
    return node;
}

// Deallocate memory of a linked list:
void freeLinkedList(LinkedList* linkedList)
{
    Node* node;
    while((node = linkedList->head) != NULL)
    {
        linkedList->head = linkedList->head->next;
        free(node);
    }
    free(linkedList);
}

/*
int delete_ll_node(ll **list_info, void *data,int (*function)(const char *, const char*){
	
    // check if we delete the start
    if ((*function)((*list_info)->head->data, data) == 0){
        ll_node *node_to_del = (*ll_info)->head;
        (*ll_info)->head = node_to_del->next;
        free(node_to_del);
        return 1;
    }

    // find the previous node of the one to be deleted
    ll_node *prev = search_list_prev_node(list, data, function);
    
    // if prev is null then node to be deleted is head
    if (prev == NULL) {
        printf("The node does not exist in the list.\n");
        return 1;
    }

    // link previous node with the next of the one to delete
    ll_node *node_to_del = prev->next;
    prev->next = node_deleted->next;
    
    // free memory
    free(node_to_del);
    return 1;
}*/
