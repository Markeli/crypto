#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include <list.h>

List* CreateList(void)
{
    List *res = (List*)malloc(sizeof(List));
    res->head = NULL;
    res->elemsCount = 0;
}

Node* NewListItem(void)
{
    Node *result = (Node*)malloc(sizeof(Node));
    result->next = 0;
    return result;
}

void DeleteItems(Node* item)
{
    if (item)
    {
        if (item->next)
        {
            DeleteItems(item->next);
        }
        free(item);
    }
}

void DeleteList(struct List *list)
{
    if (list)
    {
        DeleteItems(list->head);
        free(list);
    }
}

int InsertItem(List *list, int socketFD, char userName[PARAMETRS_LENGTH])
{
   Node *item = NewListItem();
   if ( item == NULL ) return 0;
   item->socketFD = socketFD;
   strcpy( item->userName, userName);
   item->next = list->head;
   list->head = item;
   list->elemsCount++;
   return 1;
}

int InsertItemUniq(List *list, int socketFD, char userName[PARAMETRS_LENGTH])
{
   if ( list )
   {
      Node *node;
      for ( node = list->head; node != NULL; node = node->next )
      {
         if (strcmp(node->userName, userName) == 0)
         {
            return 0;
         }
      }
      return InsertItem(list, socketFD, userName);
   }
   else
   {
      return 0;
   }
}

int DeleteItem( List *list, int socketFD )
{
   Node *item, *previousItem = 0;
   for (  item = list->head; item != NULL; previousItem = item, item = item->next )
   {
      if ( item->socketFD == socketFD )
      {
         if ( previousItem != 0 )
         {

             previousItem->next = item->next;
         }
         else
         {

             list->head = item->next;
         }
         free( item );
         list->elemsCount--;
         return 1;
      }
   }
   return 0;
}


void PrintList (  List *list )
{
   Node *item;
   printf( "Size=%d\n", list->elemsCount );
   for ( item = list->head; item != NULL; item = item->next )
   {
      printf("(%d,%s) -> ", item->socketFD, item->userName);
   }
   printf( "NULL\n" );
}

int FindItem( List *list, int socketFD)
{
   Node *item;
   for ( item = list->head; item != NULL; item = item->next )
   {
      if ( item->socketFD == socketFD )
      {
         return 1;
      }
   }
   return 0;
}
