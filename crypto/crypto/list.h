#ifndef LIST_H
#define LIST_H

#include <common.h>

typedef int mkey_t;
typedef int value_t;
typedef struct Node Node;
typedef struct List List;

struct Node
{
    mkey_t key;
    char userName[PARAMETRS_LENGTH];
    Node* next;
};

struct List
{
    Node *head;
    int elemsCount;
};

List* CreateList(void);
Node* NewListItem(void);
void DeleteItems(Node* item);
void DeleteList(List *list);
int InsertItem(List *list, mkey_t key, char[PARAMETRS_LENGTH]);
int InsertItemUniq(List *list, mkey_t key, char[PARAMETRS_LENGTH]);
int DeleteItem(List *list, mkey_t key );
void PrintList(List *list );
int FindItem(List *list, mkey_t key);

#endif // LIST_H
