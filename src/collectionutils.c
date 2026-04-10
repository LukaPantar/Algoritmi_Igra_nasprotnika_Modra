#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "collectionutils.h"

LinkedList* linkedListCreate() 
{
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if (list) 
    {
        list->head = NULL;
        list->size = 0;
    }
    return list;
}

void linkedListPushBack(LinkedList* list, void* data) 
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;

    if (list->head == NULL) 
    {
        list->head = new_node;
    }
    else
    {
        Node* temp = list->head;
        while (temp->next)
            temp = temp->next;

        temp->next = new_node;
    }
    list->size++;
}

bool likedListRemoveNode(LinkedList* list, Node* target, bool free_data) 
{
    if (!list || !list->head || !target) 
        return false;

    Node* curr = list->head;
    Node* prev = NULL;

    while (curr != NULL) 
    {
        if (curr == target) 
        {
            if (prev == NULL) 
                list->head = curr->next;
            else
                prev->next = curr->next;

            if (free_data) 
                free(curr->data);
            
            free(curr);
            list->size--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

void linkedlistDestroy(LinkedList* list, bool free_data) 
{
    Node* curr = list->head;
    while (curr != NULL) 
    {
        Node* next = curr->next;
        if (free_data) 
            free(curr->data);
        
        free(curr);
        curr = next;
    }
    free(list);
}

BinaryTree* binaryTreeCreate() 
{
    BinaryTree* tree = (BinaryTree*)malloc(sizeof(BinaryTree));
    if (tree) 
    {
        tree->root = NULL;
        tree->size = 0;
    }
    return tree;
}

void _destroyNode(BinaryTreeNode* node, bool free_data) 
{
    if (!node)
        return;

    _destroyNode(node->left, free_data);
    _destroyNode(node->right, free_data);
    if (free_data)
        free(node->data);

    free(node);
}

void binaryTreeDestroy(BinaryTree* tree, bool free_data) 
{
    if (!tree) 
        return;
    
    _destroyNode(tree->root, free_data);
    free(tree);
}

BinaryTreeNode* binaryTreeBSTInsert(BinaryTree* tree, void* data, CompareFunc compare)
{
    BinaryTreeNode* new_node = (BinaryTreeNode*)malloc(sizeof(BinaryTreeNode));
    new_node->data = data;
    new_node->left = new_node->right = NULL;

    if (tree->root == NULL)
    {
        tree->root = new_node;
        new_node->parent = NULL;
    }
    else
    {
        BinaryTreeNode* curr = tree->root;
        BinaryTreeNode* parent = NULL;
        while (curr != NULL)
        {
            parent = curr;
            if (compare(data, curr->data) < 0)
                curr = curr->left;
            else
                curr = curr->right;
        }
        
        new_node->parent = parent;
        if (compare(data, parent->data) < 0)
            parent->left = new_node;
        else
            parent->right = new_node;
    }

    tree->size++;
    return new_node;
}

BinaryTreeNode* _findMin(BinaryTreeNode* node)
{
    while (node && node->left) node = node->left;
    return node;
}

void binaryTreeBSTRemoveNode(BinaryTree* tree, BinaryTreeNode* node, bool free_data)
{
    if (!tree || !node)
        return;

    // Two children
    if (node->left && node->right)
    {
        BinaryTreeNode* successor = _findMin(node->right);
        void* temp = node->data;
        node->data = successor->data;
        successor->data = temp;
        binaryTreeBSTRemoveNode(tree, successor, free_data);
        return;
    }

    // One or no children
    BinaryTreeNode* child = (node->left) ? node->left : node->right;

    if (child) 
        child->parent = node->parent;

    if (node->parent == NULL) 
    {
        tree->root = child;
    }
    else
    {
        if (node->parent->left == node)
            node->parent->left = child;
        else 
            node->parent->right = child;
    }

    if (free_data)
        free(node->data);

    free(node);
    tree->size--;
}

void _printTreeRecursive(BinaryTreeNode* node, int space, PrintDataCallback printCallback)
{
    const int LEVEL_GAP = 10;
    if (node == NULL) return;

    space += LEVEL_GAP;

    // process right
    _printTreeRecursive(node->right, space, printCallback);
    
    for (int i = LEVEL_GAP; i < space; i++) 
        printf(" ");
    
    if (printCallback)
        printCallback(node->data);
    else
        printf("[No Callback]");

    // Process Left child
    _printTreeRecursive(node->left, space, printCallback);
}

void binaryTreePrint(BinaryTree* tree, PrintDataCallback printCallback)
{
    if (!tree || !tree->root)
    {
        printf("The tree is empty.\n");
        return;
    }
    _printTreeRecursive(tree->root, 0, printCallback);
    printf("\n");
}
