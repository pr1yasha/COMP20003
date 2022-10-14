#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // atoi
#include <assert.h>
#include <math.h>

#include "quadTree.h"
#include "data.h"
#include "list.h"

int main(int argc, char **argv) {
    FILE* inputFile = NULL;
    FILE* outputFile = NULL;

    if (argc > 1){
        inputFile = getInputFile(argv[2]);
        outputFile = makeOutputFile(argv[3]);
    }

    // Parse information from argv
    point2D bottomLeft = createPoint2D(strtold(argv[4],NULL),strtold(argv[5],NULL));
    point2D topRight = createPoint2D(strtold(argv[6],NULL), strtold(argv[7],NULL));
    rectangle2D* rootRectangle = createRectangle(bottomLeft, topRight);
    struct quadTreeNode* root = NULL;
    root = (quadTreeNode*)malloc(sizeof(quadTreeNode));
    root->rectangle = rootRectangle;
    root->state = WHITE;
    
    readInputFile(inputFile, root);
    readStringsDict4(outputFile, root);

    fclose(outputFile);
    fclose(inputFile);

    free_root(root);

    return(0);
    }
