#include "../include/list.h"

int compare_tuples(Tuple tuplefirst, Tuple tuplesecond){
    if ( tuplefirst.port_num == tuplesecond.port_num && tuplefirst.ip_address.s_addr == tuplesecond.ip_address.s_addr) {
        return 0;
    }else{
        return 1;
    }
}

List *initialize_list(void){
    List *list = malloc(sizeof(List));
    list->head = NULL;
    list->size = 0;
    return list;
}

Listnode *add_listnode(List *list, Tuple data){

    // check if node  exists
    if (search_listnode(list, data) != NULL) {
        return NULL;
    }

    // allocate memory for node
    List_node *node = (Listnode*)malloc(sizeof(Listnode));
    new_node->tuple = data;
    new_node->next = list->head;

    // change head pointer as new node is added at the beginning
    list->headlist = newnode;
    (list->size)++;

    return node;
}

void print_list(List *list){
    //printf("size of list %d\n", list->size);
    Listnode *cur = list->headlist;

    while (cur != NULL){
        printf("Port: %d , IP: %d \n", cur->tuple.ip_address.s_addr,cur->tuple.port_num);
        cur = cur->next;
    }
}

Listnode *search_listnode(List *list, Tuple data){
    Listnode *cur = list->headlist;

    while (cur != NULL) {
        if (compare_tuples(current->tuple, data) == 0){
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

int delete_listnode(List *list, Tuple data){

    // store headlist node
    Listnode *cur = list->headlist, *previous;

    // node to be deleted is head
    if (cur != NULL && compare_tuples(current->tuple, data) == 0){
        list->headlist = cur->next;
        (list->size)--;
        free(cur);
        return 0;
    }

    // search for the key to be deleted
    // previous->next node will change
    while (cur != NULL && compare_tuples(cur->tuple, data) == 1) {
        previous = current;
        current = current->next;
    }

    // if there is no such node in the list
    if (current == NULL) {
        return 1;
    }

    (list->size)--;
    // unlink the node from linked list
    previous->next = cur->next;

    // free memory
    free(cur);
    return 0;
}

void delete_list(List *list){
    Listnode *cur = list->head;
    Listnode *next;

    while (cur != NULL){
        next = cur->next;
        free(cur);
        cur = next;
    }
    free(list);
}
