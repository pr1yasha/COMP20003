/* dict1.c
This program allows users to search footpaths by their address and return
relevant data given a CSV input.
*/

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // atoi

#include "data.h"
#include "list.h"

int main(int argc, char **argv) {
    FILE* inputFile = NULL;
    FILE* outputFile = NULL;

    if (argc > 3){
    inputFile = getInputFile(argv[2]);
    outputFile = makeOutputFile(argv[3]);
    }
    
    if (inputFile == NULL || outputFile == NULL){
        printf("null");
        return(1);
    }

    int nodeCount = 0;
    struct Node* head = readInputFile(inputFile, &nodeCount);
    fclose(inputFile);
    read_strings(head, outputFile);
    fclose(outputFile);
    freeList(head);
    return 0;
}
