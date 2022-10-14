#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // atoi
#include <assert.h>
#include <math.h>

#include "quadTree.h"
#include "list.h"
#include "data.h"

// Initialises a quad tree node
struct quadTreeNode* initialiseNode(point2D bottomLeft, point2D topRight){
    struct quadTreeNode* newNode = malloc(sizeof(struct quadTreeNode));
    newNode->rectangle = makeRectangle(bottomLeft, topRight);
    newNode->state = WHITE;
    newNode->NE = NULL;
    newNode->SE = NULL;
    newNode->NW = NULL;
    newNode->SW = NULL;
    return(newNode);
}

// Function to free quad tree node
void free_node(quadTreeNode* node){
    free(node->rectangle);
    if (node->state == BLACK){
        freeList(node->data.footpathNodes);
    }
    free(node);
}

// Function to free quad tree
int free_root(quadTreeNode* root_node){
    if (root_node->state == GREY) {
        free_root(root_node->SW);
        free_root(root_node->SE);
        free_root(root_node->NE);
        free_root(root_node->NW);
    }
    free_node(root_node);
    return(0);
}

// Add a footpath to the datapoint. Taken from assignment 1
void addItemToList(struct quadTreeNode* baseNode, struct Footpath footpath){

    struct listNode* currNode = baseNode->data.footpathNodes;
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

// Splits node into child nodes
int splitNode(quadTreeNode* node){
    // initialise the child nodes
    point2D SWBottomLeft = node->rectangle->bottomLeft;
    point2D SWTopRight = createPoint2D(((node->rectangle->topRight.x)+(node->rectangle->bottomLeft.x))/2, ((node->rectangle->topRight.y)+(node->rectangle->bottomLeft.y))/2);
    node->SW = initialiseNode(SWBottomLeft, SWTopRight);
    point2D NEBottomLeft = createPoint2D(((node->rectangle->topRight.x)+(node->rectangle->bottomLeft.x))/2, ((node->rectangle->topRight.y)+(node->rectangle->bottomLeft.y))/2);
    point2D NETopRight = node->rectangle->topRight;
    node->NE = initialiseNode(NEBottomLeft, NETopRight);
    point2D SEBottomleft = createPoint2D(((node->rectangle->topRight.x)+(node->rectangle->bottomLeft.x))/2, node->rectangle->bottomLeft.y);
    point2D SETopRight = createPoint2D(node->rectangle->topRight.x, ((node->rectangle->topRight.y)+(node->rectangle->bottomLeft.y))/2);
    node->SE = initialiseNode(SEBottomleft, SETopRight);
    point2D NWBottomLeft = createPoint2D(node->rectangle->bottomLeft.x, ((node->rectangle->topRight.y)+(node->rectangle->bottomLeft.y))/2);
    point2D NWTopRight = createPoint2D(((node->rectangle->topRight.x)+(node->rectangle->bottomLeft.x))/2, (node->rectangle->topRight.y));
    node->NW = initialiseNode(NWBottomLeft, NWTopRight);

    // Shifts the data point of the current node to its proper leaf node.
    int quadrant = determineQuadrant(node->rectangle, node->data.location);
    if (quadrant == NE_VAL){
        insertIntoQuadTree(node->NE, (node->data));
        return(0);
    }
    else if (quadrant == NW_VAL){
        insertIntoQuadTree(node->NW, (node->data));
        return(0);
    }
    else if (quadrant == SW_VAL){
        insertIntoQuadTree(node->SW, (node->data));
        return(0);
    }
    else {
        insertIntoQuadTree(node->SE, (node->data));
        return(0);
    }

    return(0);

}

// Inserts datapoints into quad trees
int insertIntoQuadTree(quadTreeNode* node, dataPoint data){

    // exits the function if the data does not fit inside the rectangle of the current node.
    if (inRectangle(node->rectangle, data.location) == FALSE){
        return(1);
    }

    // Exits if the rectangle has turned into a singular point to avoid infinite recursion 
    if (fabs(node->rectangle->bottomLeft.x - node->rectangle->topRight.x) < 0.000000000001 && fabs(node->rectangle->bottomLeft.y - node->rectangle->topRight.y) <  0.000000000001 ) {
        return(0);
    } 

    // if the node is a white leaf node, we can copy our data to it.
    if (node->state == WHITE){
        node->state = BLACK;
        node->data = data;
        return(0);
    }

    // if the node is already occupied with a data point identical to us, we can simply append our new footpath instead of a datapoint.
    else if (node->data.location.x == data.location.x && node->data.location.y == data.location.y){
       Footpath *footpath = data.footpath;
        addItemToList(node, *footpath); 
        return(0);
    }

    // we have reached a node with a data point already. We need to split it
    if (node->state != GREY){

        // This function pushes the data of 'node' down to its appropriate child
        splitNode(node);
    }

    // Marks the node as an internal node
    node->state = GREY;

    // Now that the internal node has been taken care of, append our new datapoint
    // Determine which quadrant it should be pushed to and then call the function recursively
    int quadrant = determineQuadrant(node->rectangle, data.location);
    if (quadrant == NE_VAL){
        insertIntoQuadTree(node->NE, data);
    }
    else if (quadrant == NW_VAL){
        insertIntoQuadTree(node->NW, data);
    }
    else if (quadrant == SW_VAL){
        insertIntoQuadTree(node->SW, data);
    }
    else {
        insertIntoQuadTree(node->SE, data);
    }
    return(1);
}

// Reads input file and makes quad tree
struct quadTreeNode* readInputFile(FILE* inputFile, quadTreeNode* root){
    char *line = NULL;
    size_t len = 0;
    ssize_t read; // should be ssize_t
    int lineCounter = 0;
    
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

            // Re-introduce commas into fields and remove quotation marks
           editField(currFootpath.clue_sa, len);
            point2D startPoint = createPoint2D(strtold(array[16], NULL), strtold(array[15], NULL));
            point2D endPoint = createPoint2D(strtold(array[18], NULL), strtold(array[17], NULL));
            
            // inserts both the starting point and ending point of each Footpath into the quad tree.
            dataPoint startDataPoint = createDataPoint(&currFootpath, startPoint);
            dataPoint endDataPoint = createDataPoint(&currFootpath, endPoint);
            insertIntoQuadTree(root, startDataPoint);
            insertIntoQuadTree(root, endDataPoint);

        }
    }
    
    free(line);
    return(root);
}

//Taken from my submission of ass.1 
void read_strings(FILE* outputFile, quadTreeNode* head) {
    char ss[MAX_LEN];
    strcpy(ss, "");
    
    // For each address in stdin, search linked list for duplicate footpaths
    while (fgets(ss, MAX_LEN, stdin)) {
        ss[strcspn(ss, "\n")] = 0;
        interpret(ss, head, outputFile);
        strtok(ss, "\n");
    }
}

// Parse the info given in stdin so it can be read by the functions
void interpretDict4(char ss[MAX_LEN], quadTreeNode* head, FILE* outputFile){
   char *coord = strtok(ss," ");
   long double bottomLeft_x, bottomLeft_y, topRight_x, topRight_y;
   int i;

    // Loop to parse and print coordinates to stdout and outputfile
    for (i=0; i<4; i++){
        // The bottom left longitude
       if (i==0){
           bottomLeft_x = strtold(coord, NULL);
            printf("%s ", coord);
            fprintf(outputFile, "%s ", coord);
            coord = strtok(NULL, " ");
        // The bottom left latitude
       } else if (i==1){
            bottomLeft_y = strtold(coord, NULL);
            fprintf(outputFile, "%s ", coord);
            printf("%s ", coord);
            coord = strtok(NULL, " ");
        // The top right longitude
       } else if (i==2){
            topRight_x =  strtold(coord, NULL);
            fprintf(outputFile, "%s ", coord);
            printf("%s ", coord);
            coord = strtok(NULL, " ");
       } // The top right latitude
       else {
            topRight_y = strtold(coord, NULL);
            fprintf(outputFile, "%s", coord);
            printf("%s -->", coord);
            coord = strtok(NULL, " ");
       }
    }

    fprintf(outputFile, "\n");

    // Generate the query in a usable form
    point2D bottomLeftBound = createPoint2D(bottomLeft_x, bottomLeft_y);
    point2D topRightBound = createPoint2D(topRight_x, topRight_y);
    struct rectangle2D* currBoundary = createRectangle(bottomLeftBound, topRightBound);
    struct listNode* result = NULL;
    result = (struct listNode*)malloc(sizeof(struct listNode));
    result->next = NULL;
    result->data = createEmptyStruct();

    // Search for matches
    searchDict4(result, head, currBoundary, outputFile);
    free(currBoundary);

    // Sort matches and remove duplicates before printing
    assert(result != NULL);
    sortList(result);
    removeDuplicates(result);
    printList(result, outputFile);
    freeList(result);
    printf("\n");

}


// Taken from my submission of ass.1 
void readStringsDict4(FILE* outputFile, quadTreeNode* head) {
    char ss[MAX_LEN];
    strcpy(ss, "");
    
    // For each address in stdin, search linked list for duplicate footpaths
    while (fgets(ss, MAX_LEN, stdin)) {
        ss[strcspn(ss, "\n")] = 0;
        interpretDict4(ss, head, outputFile);
        strtok(ss, "\n");
    }
}


int search(quadTreeNode* head, point2D searchQuery, FILE* outputFile){
   struct quadTreeNode* current = head;

    // This is a internal node, so search durther down recursively
    if (current->state == GREY){
        int direction = determineQuadrant(current->rectangle, searchQuery);
        if (direction==NE_VAL){ 
            printf(" NE");
            search(current->NE, searchQuery, outputFile); }
        if (direction==SE_VAL){ 
            printf(" SE");
            search(current->SE, searchQuery, outputFile); }
        if (direction==SW_VAL){ 
            printf(" SW");
            search(current->SW, searchQuery, outputFile); }
        if (direction==NW_VAL){ 
            printf(" NW");
            search(current->NW, searchQuery, outputFile); }
    }

    // we can't look here, this is a leaf node. exit the search
    if (current->state == WHITE){
        return(1);
    }
    
    // This is a node with an existing data point
    if (current->state == BLACK){
        // Check if the data point equals to our query 
        if (fabs(searchQuery.x - current->data.location.x) < 0.00000000001 && fabs(searchQuery.y - current->data.location.y) < 0.0000000001){
            // Sort output based on footpath.id before printing
            fprintf(outputFile, "%.12Lf %.12Lf \n", searchQuery.x, searchQuery.y);
            sortList(current->data.footpathNodes);
            printList(current->data.footpathNodes, outputFile);
        }
        else {
           return(0);
        }
   }

return(0);
}

void interpret(char ss[MAX_LEN], quadTreeNode* head, FILE* outputFile){
   char *coord = strtok(ss," ");
   long double longitude;
   long double latitude;
   int i;
   for (i=0; i<2; i++){
        if (i==0){
            // Query's longitude
            longitude = strtold(coord, NULL);
            printf("%s ", coord);
            coord = strtok(NULL, " ");

       }
       else {
            //Query's latitude
            latitude = strtold(coord, NULL);
            printf("%s -->", coord);
            coord = strtok(NULL, " ");
       }
   }
   // Search for query
   point2D searchQuery = createPoint2D(longitude, latitude);
   search(head, searchQuery, outputFile);
   printf("\n");
}

// Search the quad tree to return nodes within a given boundary.
// Inspired by https://en.wikipedia.org/wiki/Quadtree#Query_range
int searchDict4(listNode* result, quadTreeNode* head, rectangle2D* rectangle, FILE* outputFile){
   struct quadTreeNode* current = head;
   // Exit function if node doesn't overlap with rectangle given in search query
   if (rectangleOverlap(head->rectangle, rectangle) == FALSE){
       return(1);
   }

    // This is an internal node, keep looking recursively.
   if (current->state == GREY){
       if (rectangleOverlap(current->SW->rectangle, rectangle) == TRUE){ 
           if (current->SW->state != WHITE){
            printf(" SW");
           }
           searchDict4(result, current->SW, rectangle, outputFile); }
       if (rectangleOverlap(current->NW->rectangle, rectangle) == TRUE){ 
           if (current->NW->state != WHITE){
            printf(" NW");
           }
           searchDict4(result, current->NW, rectangle, outputFile); }
       if (rectangleOverlap(current->NE->rectangle, rectangle) == TRUE){ 
           if (current->NE->state != WHITE){
            printf(" NE");
           }
           searchDict4(result, current->NE, rectangle, outputFile); }
       if (rectangleOverlap(current->SE->rectangle, rectangle) == TRUE){ 
           if (current->SE->state != WHITE){
            printf(" SE");
           }
           searchDict4(result, current->SE, rectangle, outputFile); }
   }

    // We can't look here, this is a leaf node. Exit the search
   if (current->state == WHITE){
       return(1);
   }

    // A leaf node with data
   if (current->state == BLACK){

        // Check the points in the node also fall in the boundary of the query
        if (inRectangle(rectangle, current->data.location) == TRUE){

            // Append all footpaths in the dataPoint of the node to a final linked list we can print and sort
            struct listNode *temp = current->data.footpathNodes;
            while (temp != NULL) {
                addFootpathToList(result, temp->data, outputFile);
                temp = temp->next;
            }

        }
    }
    return(0);
   
return(0);
}



