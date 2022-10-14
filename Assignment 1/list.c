clo#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "data.h"

#define MAX_STR_LEN 128
struct Node {
    struct Footpath data;
    struct Node* next;
};

// prints the number of records with matching addresses
void printMatchCount(int matches, char ss[MAX_STR_LEN]){
    if (matches == 0){
        printf("%s --> NOTFOUND\n", ss);
    }
    else {
        printf("%s --> %d\n", ss, matches);
    }

}

// Function to 'search' linked list for footpaths with the same address
int traverseListToFindDuplicates(struct Node* head, char ss[MAX_STR_LEN], FILE* outputFile){
    struct Node* current = head;
    int matchCount = 0;
    // loop through ;inked list
    while (current->next != NULL){
       if (current->data.footpath_id != 0){
           // check if the current record matches the input
           if (strcmp(current->data.address , ss) == 0){
                printStruct(current->data, outputFile);
                matchCount++;
            }

        }
        current = current->next;
    }
    // check if the last node matches the input 
    if (strcmp(current->data.address , ss) == 0){
        printStruct(current->data, outputFile);
        matchCount++;
    }
            
    return(matchCount);
}

// Reads addresses being searched from stdin
void read_strings(struct Node* head, FILE* outputFile) {
    char ss[MAX_STR_LEN];
    strcpy(ss, "");
    int matches = 0;
    
    // For each address in stdin, search linked list for duplicate footpaths
    while (fgets(ss, MAX_STR_LEN, stdin)) {
        ss[strcspn(ss, "\n")] = 0;
        fprintf(outputFile, "%s\n", ss);
        matches = traverseListToFindDuplicates(head, ss, outputFile);
        strtok(ss, "\n");
        printMatchCount(matches, ss);
    }
}

// Swaps data of two nodes
void swapNodes(struct Node* node1, struct Node* node2){
    struct Footpath tempFootpath = node1->data;
    node1->data = node2->data;
    node2->data = tempFootpath;
}

// prints results of search matches to output file
void printToFile(char ss[MAX_STR_LEN], struct Node* result, FILE* outputFile){
    fprintf(outputFile, "%s\n", ss);
    printStruct(result->data, outputFile);
}

// prints results of search match to stdout
void printMatch(char ss[MAX_STR_LEN], struct Node* result){
    printf("%s --> %g\n", ss, result->data.grade1in);
}

// sorts the array of pointers to nodes using Bubble Sort
void sortLinkedList(struct Node* pointerArray[], int nodeCount){
    int i, j;
    for (i=0; i<nodeCount-1; i++){
        for (j=0; j<nodeCount-i-1; j++){
            if (pointerArray[j]->data.grade1in > pointerArray[j+1]->data.grade1in){
                swapNodes(pointerArray[j], pointerArray[j+1]);
            }   
        }
    }
    i = 0;
}

// Adds pointer to node to end of linked list  
void addItemToList(struct Node* head, struct Footpath footpath){
    struct Node* currNode = head;
    // Reach end of list
    while (currNode->next != NULL){
        currNode = currNode->next;
    }
    // Append item to the end
    currNode->next = (struct Node*)malloc(sizeof(struct Node));
    currNode = currNode->next;
    currNode->next = NULL;
    currNode->data = footpath;
}

// Reads the input dataset and creates a linked list
struct Node* readInputFile(FILE* inputFile, int* numberOfNodes){
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int lineCounter = 0;
    
    // create head of the linked list and initialise to null
    struct Node* head = NULL;
    head = (struct Node*)malloc(sizeof(struct Node));
    head->next = NULL;
    struct Footpath footpath = {};
    head->data = footpath;
    int nodeCount = 0;

    // Get each row from input csv 
    while ((read = getline(&line, &len, inputFile)) != -1) {
    // Remove commas from each row so they don't get parsed incorrectly
    removeCommas(line, strlen(line));
        lineCounter++;
        char *array[20];
        int i=0;
        char *element;
        char *temp = line; 

        // Create array for each row in the csv, with each cell becoming 
        // struct attributes of Footpath
        while( (element = strsep(&temp,",")) != NULL ){
            array[i] = element;
            i++;
        }
  
        i = 0;

        // Conditional statement to prevent header row from being 
        // initialised as a struct in the list
        if (lineCounter > 1){
            struct Footpath currFootpath = createStruct(array);
            int len = strlen(currFootpath.clue_sa);
            // Re-introduce commas into fields and remove quotation marks
            editField(currFootpath.clue_sa, len);
            nodeCount++;
            // Appends new footpath from the CSV row to the linked list 
            addItemToList(head, currFootpath);
        }
    }
    
    free(line);
    *numberOfNodes = nodeCount;
    return(head);
}

// Searches linked list for footpath with closest gradelin to given value, 
// using binary search
struct Node* binarySearch(struct Node* pointerArray[], double desiredGradelin, 
                        int count, FILE* outputFile){
    int start = 0;
    int end = count-1;
    int middle;

    // While sub-array is larger than 2 elements
    // Searches for 2 closest elements to the target value
    while (end - start > 1){
        middle = (start+end)/2;
        if (pointerArray[middle]->data.grade1in == desiredGradelin){
            return(pointerArray[middle]);
        }
        if (desiredGradelin > pointerArray[middle]->data.grade1in){
            start = middle;
        }

        if (desiredGradelin < pointerArray[middle]->data.grade1in){
            end = middle;
        }

    }

    // Checks if value on left or right is closer to target
    double left = fabs(desiredGradelin - pointerArray[start]->data.grade1in);
    double right = fabs(desiredGradelin - pointerArray[end]->data.grade1in);
    if (left<right){
        return(pointerArray[start]);
    }
    else{
        return(pointerArray[end]);
    }

}



// Reads addresses being searched from stdin
void getGrade1inMatches(struct Node* pointerArray[], struct Node* head, 
                        FILE* outputFile, int count) {
    char ss[MAX_STR_LEN];
    
    // get each line of input
    while (fgets(ss, MAX_STR_LEN, stdin)) {
        strtok(ss, "\n");
        // search linked list for closest grade1in to that read by the while loop
        // print and output results
        struct Node* result = binarySearch(pointerArray, atof(ss), count, outputFile);
        printMatch(ss, result);
        printToFile(ss, result, outputFile);
    }
}

// Creates an array of pointers to nodes in the linked list
void createArray(struct Node* head, int count, FILE* outputFile){
    int i = 0;
    struct Node* pointerArray[count];
    struct Node* current = head;
    // Loop through linked list and copy each node to array
    while (current != NULL){
        if (current->data.footpath_id != 0){
            pointerArray[i] = current;
            i++;
        }
        current = current->next;
    }
    i = 0;
    // sort the array and search for closest grade1in match
    sortLinkedList(pointerArray, count);
    getGrade1inMatches(pointerArray, head, outputFile, count);
}

// Free the linked list created
void freeList(struct Node* head){
    struct Node* current = head;
    while (head != NULL){
        current = head; 
        head = head->next;
        free(current);
    }
}
