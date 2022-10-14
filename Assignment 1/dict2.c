/* dict2.c
This program allows users to search for the footpath closest to the specified
grade1in value and returns relevant data given a CSV input.
*/

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // atoi

#include "data.h"
#include "list.h"

/*#define MAX_LEN 128
#define MAX_STRS 500
#define MAX_STR_LEN 128*/


int main(int argc, char **argv) {
    FILE* inputFile = NULL;
    FILE* outputFile = NULL;

    if (argc > 3){
    inputFile = getInputFile(argv[2]);
    outputFile = makeOutputFile(argv[3]);
    }

    if (inputFile == NULL){
        printf("null");
        return(1);
    }

    int nodeCount = 0;
    struct Node* head = readInputFile(inputFile, &nodeCount);
    createArray(head, nodeCount, outputFile);
    fclose(inputFile);
    fclose(outputFile);
    freeList(head);
    return 0;
}
