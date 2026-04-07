#ifndef COLLECTIONUTILS_H
#define COLLECTIONUTILS_H

#include <stdbool.h>
#include <stdlib.h>

/* --------- Linked List --------- */

typedef struct Node {
    void* data;
    struct Node* next;
} Node;

typedef struct _LinkedList_ {
    Node* head;
    int size;
} LinkedList;

/* Creation and deletion */
LinkedList* linkedListCreate();
void  linkedlistDestroy(LinkedList* list, bool free_data);

/* Operations */
void  linkedListPushBack(LinkedList* list, void* data);
bool  likedListRemoveNode(LinkedList* list, Node* target, bool free_data);

/* Iteration Helper */
#define LINKEDLIST_FOREACH(node, list) for (Node* node = (list)->head; node != NULL; node = node->next)

/* ------------------------------- */

#endif