#ifndef ASSIGNMENT_1_LINKEDLIST_H
#define ASSIGNMENT_1_LINKEDLIST_H

typedef struct Node
{
    void* item;
    struct Node* next;
} Node;

typedef struct ll
{
    Node* head;
    Node* tail;
} ll;

Node* initialize_node(void* records);
ll* initialize_linked_list(Node* node);
void append_ll(ll* linked_list, Node* node);
Node* pop_start(ll* linked_list);
void freeLinkedList(ll* linked_list);
#endif //ERGASIA_1_LINKEDLIST_H
