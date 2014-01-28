#ifndef LIST_H
#define LIST_H

#include <common.h>

typedef struct Node Node;
typedef struct List List;

struct Node
{
    int socketFD;
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
int InsertItem(List *list, int socketFD, char[PARAMETRS_LENGTH]);
int InsertItemUniq(List *list, int socketFD, char[PARAMETRS_LENGTH]);
int DeleteItem(List *list, int socketFD );
void PrintList(List *list);
int FindItem(List *list, int socketFD);

#endif // LIST_H
