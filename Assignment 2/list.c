#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // atoi
#include <assert.h>
#include <math.h>

#include "quadTree.h"
#include "list.h"
#include "data.h"

// Sorting function using bubble sort. https://www.tutorialspoint.com/data_structures_algorithms/bubble_sort_algorithm.htm
// Sorts the linked list by footpath ID when there are multiple footpaths per query.
void sortList(struct listNode* head) {  
    struct listNode *currentNode = head, *index = NULL;  
    struct Footpath temp;  
        if (head == NULL) {  
            return;  
        }  

        else {  
            while(currentNode != NULL) {  
                index = currentNode->next;   
                while(index != NULL) {  
                    // swaps footpaths by footpath ID in ascending order
                    if (currentNode->data.footpath_id > index->data.footpath_id) {  
                        temp = currentNode->data;  
                        currentNode->data = index->data;  
                        index->data = temp;  
                    }  
                    index = index->next;  
                }  
                currentNode = currentNode->next;  
            }      
        }  
}   

// Prints linked list of matched footpaths
void printList(listNode* results, FILE* outputFile){
    listNode* temp = results;
   	while ( temp != NULL ) {
           if (temp->data.footpath_id != 0){
           printStruct(temp->data, outputFile);

           }
           temp = temp->next;
       }
}

// Free linked list
void freeList(struct listNode* head){
    struct listNode* current = head;
    while (head != NULL){
        current = head; 
        head = head->next;
        free(current);
    }
}

// creates the head node
listNode* createHead(Footpath data){
    struct listNode* head = NULL;
    head = (struct listNode*)malloc(sizeof(struct listNode));
    head->next = NULL;
    head->data = data;
    return(head);
}


// Add footpath to list of results. Taken from my assignment 1
void addFootpathToList(struct listNode* baseNode, struct Footpath footpath, FILE* outputFile){
    struct listNode* currNode = baseNode;
    // Reach end of list
    while (currNode->next != NULL){
        currNode = currNode->next;
    }
    // Append item to the end
    currNode->next = (struct listNode*)malloc(sizeof(struct listNode));
    currNode = currNode->next;
    currNode->next = NULL;
    currNode->data = footpath;
}

// Removes the duplicate footpaths in a linked list
void removeDuplicates(listNode* head){
    struct listNode* temp = head;
       	while ( temp->next != NULL) {
                if (temp->data.footpath_id != 0){
                    if (temp->data.footpath_id == temp->next->data.footpath_id){
                        temp->next->data.footpath_id = 0;
                    }
                }
            temp = temp->next;
        }

}
// concatenates two linked lists
void append(listNode* results, listNode* additionalPoints){
    if (results->next == NULL)
        results->next = additionalPoints;
    else
        append(results->next,additionalPoints);
}

