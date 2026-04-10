#ifndef COLLECTIONUTILS_H
#define COLLECTIONUTILS_H

/* --------- Linked List --------- */

typedef struct Node 
{
    void* data;
    struct Node* next;
} Node;

typedef struct _LinkedList_ 
{
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
/* --------- Binary Tree --------- */

typedef struct BinaryTreeNode 
{
    void* data;
    struct BinaryTreeNode* parent;
    struct BinaryTreeNode* left;
    struct BinaryTreeNode* right;
} BinaryTreeNode;

typedef struct _BinaryTree_
{
    BinaryTreeNode* root;
    int size;
} BinaryTree;

/* Creation and deletion */
BinaryTree* binaryTreeCreate();
void binaryTreeDestroy(BinaryTree* tree, bool free_data);

/* Operations */
// Not needed:
// BinaryTreeNode* binaryTreeInsertLeaf(BinaryTree* tree, BinaryTreeNode* parent, void* data, bool left);
// BinaryTreeNode* binaryTreeInsertAbove(BinaryTree* tree, BinaryTreeNode* target, void* data, bool targetToLeft);
// void binaryTreeRemoveNode(BinaryTree* tree, BinaryTreeNode* node, bool free_data);

/* BST Operations */
typedef int (*CompareFunc)(void* a, void* b);
BinaryTreeNode* binaryTreeBSTInsert(BinaryTree* tree, void* data, CompareFunc compare);
void binaryTreeBSTRemoveNode(BinaryTree* tree, BinaryTreeNode* node, bool free_data);

/* Helpers */
typedef void (*PrintDataCallback)(void* data);
void binaryTreePrint(BinaryTree* tree, PrintDataCallback printCallback);

/* ------------------------------- */

#endif