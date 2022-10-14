#ifndef _LIST_H
#define _LIST_H

#include "quadTree.h"
#include "list.h"
#include "data.h"

typedef struct listNode {
    struct listNode* next;
    Footpath data;
} listNode;

void sortList(struct listNode* head); 
void printList(listNode* results, FILE* outputFile);
void freeList(struct listNode* head);
listNode* createHead(Footpath data);
void append(listNode* results, listNode* additionalPoints);
void addFootpathToList(struct listNode* baseNode, struct Footpath footpath, FILE* outputFile);
void removeDuplicates(listNode* head);
#endif
