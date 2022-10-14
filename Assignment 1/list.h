#ifndef _LIST_H
#define _LIST_H

#include "list.h"
#include "data.h"

#define MAX_STR_LEN 500
// Struct definitions
struct Node {
    struct Footpath data;
    struct Node* next;
};

// Function prototypes
int traverseListToFindDuplicates(struct Node* head, char ss[MAX_STR_LEN], FILE* outputFile);
void read_strings(struct Node* head, FILE* outputFile);
void swapNodes(struct Node* node1, struct Node* node2);
void sortLinkedList(struct Node* pointerArray[], int nodeCount);
void addItemToList(struct Node* head, struct Footpath footpath);
struct Node* readInputFile(FILE* inputFile, int* numberOfNodes);
void createArray(struct Node* head, int count, FILE* outputFile);
void printToFile(char ss[MAX_STR_LEN], struct Node* result, FILE* outputFile);
void printMatch(char ss[MAX_STR_LEN], struct Node* result);
struct Node* binarySearch(struct Node* pointerArray[], double desiredGradelin, int count, FILE* outputFile);
void getGrade1inMatches(struct Node* pointerArray[], struct Node* head, FILE* outputFile, int count);
void freeList(struct Node* head);

#endif
