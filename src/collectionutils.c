#include "collectionutils.h"

LinkedList* linkedListCreate() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if (list) {
        list->head = NULL;
        list->size = 0;
    }
    return list;
}

void linkedListPushBack(LinkedList* list, void* data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node* temp = list->head;
        while (temp->next) temp = temp->next;
        temp->next = new_node;
    }
    list->size++;
}

bool likedListRemoveNode(LinkedList* list, Node* target, bool free_data) {
    if (!list || !list->head || !target) return false;

    Node* curr = list->head;
    Node* prev = NULL;

    while (curr != NULL) {
        if (curr == target) {
            if (prev == NULL) {
                list->head = curr->next;
            } else {
                prev->next = curr->next;
            }

            if (free_data) free(curr->data);
            free(curr);
            list->size--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

void linkedlistDestroy(LinkedList* list, bool free_data) {
    Node* curr = list->head;
    while (curr != NULL) {
        Node* next = curr->next;
        if (free_data) free(curr->data);
        free(curr);
        curr = next;
    }
    free(list);
}