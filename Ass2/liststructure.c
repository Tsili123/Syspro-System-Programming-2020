#include "liststructure.h"

ll* initialize_list(){
    ll *list_start = malloc(sizeof(ll));
    list_start->start = NULL;
	//list_start->bucket_num=0;
    return list_start;
}

ll_node* add_ll_node(ll **list_info, void *data, int size_of_structure){
	
    // Allocate memory for new node
    ll_node *new_node = malloc(sizeof(ll_node));
    
    //allocate space for the data that the node will contain
    new_node->data = malloc(size_of_structure);
    memcpy(new_node->data, data, size_of_structure);
    
    //new node is added at the beginning
    new_node->next = (*list_info)->start;
	(*list_info)->start = new_node;
	//(*list_info)->bucket_num++;
	
    return new_node;
}

/*
ll_node *search_ll_node(List **list_info, void *data, int (*function)(const char *, const char *)) {
    ll_node *current = (*list)->head;

    while (current != NULL) {
        if ((*function)(current->data, data) == 1) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

List_node *search_list_prev_node(List **list, void *data,
                                 int (*function)(void *, void *)) {
    List_node *current = (*list)->head;

    while (current != NULL && current->next != NULL) {
        if ((*function)(current->next->data, data) == 1) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Give a printing function as argument depending on the data struct
void print_list(List *list, void (*function)(void *)) {
    List_node *current = list->head;

    while (current != NULL) {
        (*function)(current->data);
        printf("\n");
        current = current->next;
    }
}

void delete_list(List **list, void (*function)(void **)) {
    List_node *current = (*list)->head;
    List_node *next;

    while (current != NULL) {
        next = current->next;
        // if a delete function was given, delete data with it
        if (*function != NULL) {
            (*function)(&current->data);
        } else {
            free(current->data);
        }

        free(current);
        current = next;
    }
    free(*list);
}*/
