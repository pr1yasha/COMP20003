#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // atoi
#include <assert.h>
#include <math.h>

#define MAX_LEN 500
#define TRUE 1
#define FALSE 0
#define WHITE 0
#define BLACK 1
#define GREY 2
#define NE_VAL 0
#define SE_VAL 1
#define SW_VAL 2
#define NW_VAL 3

typedef struct point2D {
    long double x;
    long double y;
} point2D;

typedef struct rectangle2D {
    point2D bottomLeft;
    point2D topRight; 
} rectangle2D;

typedef struct Footpath {
	int footpath_id;
	char address[MAX_LEN];       
    char clue_sa[MAX_LEN];       
    char asset_type[MAX_LEN]; 
    double deltaz;
    double distance;
    double grade1in;
    int mcc_id;
    int mccid_int;
    double rlmax;
    double rlmin;
    char segside[MAX_LEN];
    int statusid;
    int streetid;
    int street_group;
    double start_lat;
    double start_lon;
    double end_lat;
    double end_lon;
} Footpath;

typedef struct dataPoint {
    int count;
    struct point2D location;
    struct Footpath* footpath;
    struct listNode* footpathNodes;
} dataPoint;

typedef struct quadTreeNode {
    int state;
    int count;
    struct rectangle2D* rectangle;
    struct dataPoint data;
    struct quadTreeNode* NW;
    struct quadTreeNode* NE;
    struct quadTreeNode* SE;
    struct quadTreeNode* SW;
} quadTreeNode;

typedef struct listNode {
    struct listNode* next;
    Footpath data;
} listNode;


int splitNode(quadTreeNode* node);

// creates the head node of a linked list
listNode* createHead(Footpath data){
    struct listNode* head = NULL;
    head = (struct listNode*)malloc(sizeof(struct listNode));
    head->next = NULL;
    head->data = data;
    return(head);
}

// Initialises dataPoint
struct dataPoint createDataPoint(struct Footpath* footpath, struct point2D location){
    struct dataPoint newDataPoint = {};
    newDataPoint.location = location;
    newDataPoint.footpath = footpath;
    newDataPoint.footpathNodes = createHead(*footpath);
    newDataPoint.count = 1;
    return(newDataPoint);
}

// Initialised point2D
struct point2D createPoint2D(long double x, long double y){
    point2D newPoint = {};
    newPoint.x = x;
    newPoint.y = y;
    return(newPoint);
}


// Returns true if rectangles are overlapping, otherwise returns false
int rectangleOverlap(rectangle2D* rect1, rectangle2D* rect2){
    // top left rect2 is in rect1
    if (rect2->bottomLeft.x <= rect1->topRight.x && rect2->bottomLeft.x >= rect1->bottomLeft.x){
        if (rect2->topRight.y >= rect1->bottomLeft.y && rect2->topRight.y <= rect1->topRight.y){
            return(TRUE);
        }
        if (rect2->bottomLeft.y >= rect1->bottomLeft.y && rect2->bottomLeft.y <= rect1->topRight.y){
            return(TRUE);
        }
    }
    if (rect2->topRight.x <= rect1->topRight.x && rect2->topRight.x >= rect1->bottomLeft.x){
        if (rect2->topRight.y >= rect1->bottomLeft.y && rect2->topRight.y <= rect1->topRight.y){
            return(TRUE);
        }
        if (rect2->bottomLeft.y >= rect1->bottomLeft.y && rect2->bottomLeft.y <= rect1->topRight.y){
            return(TRUE);
        }
    }

    if (rect1->bottomLeft.x <= rect2->topRight.x && rect1->bottomLeft.x >= rect2->bottomLeft.x){
        if (rect1->topRight.y >= rect2->bottomLeft.y && rect1->topRight.y <= rect2->topRight.y){
            return(TRUE);
        }
        if (rect1->bottomLeft.y >= rect2->bottomLeft.y && rect1->bottomLeft.y <= rect2->topRight.y){
            return(TRUE);
        }
    }
    if (rect1->topRight.x <= rect2->topRight.x && rect1->topRight.x >= rect2->bottomLeft.x){
        if (rect1->topRight.y >= rect2->bottomLeft.y && rect1->topRight.y <= rect2->topRight.y){
            return(TRUE);
        }
        if (rect1->bottomLeft.y >= rect2->bottomLeft.y && rect1->bottomLeft.y <= rect2->topRight.y){
            return(TRUE);
        }
    }

    return(FALSE);
}


// Determines which quadrant a point 'point' falls in a rectangle 'rect'
int determineQuadrant(rectangle2D* rect, point2D point){
    long double centreX;
    long double centreY;
    centreX = (rect->topRight.x + rect->bottomLeft.x)/ 2;
    centreY = (rect->topRight.y + rect->bottomLeft.y)/ 2;

    if (point.x > centreX && point.y < centreY){
        return(SE_VAL);
    }
    else if (point.x > centreX && point.y >= centreY){
        return(NE_VAL);
    }
    else if (point.x <= centreX && point.y >= centreY){
        return(NW_VAL);
    }
    else {
        return(SW_VAL);
    }

    // won't reach here
    return(0);
}


// Creates the footpath struct, taken from Ass.1
struct Footpath createStruct(char *array[]){
    struct Footpath footpath = {};
    footpath.footpath_id = atoi(array[0]);
    strcpy(footpath.address, array[1]);
    strcpy(footpath.clue_sa, array[2]);
    strcpy(footpath.asset_type, array[3]);
    footpath.deltaz = atof(array[4]);
    footpath.distance = atof(array[5]);
    footpath.grade1in = atof(array[6]);
    footpath.mcc_id = atoi(array[7]);
    footpath.mccid_int = atoi(array[8]);
    footpath.rlmax = atof(array[9]);
    footpath.rlmin = atof(array[10]);
    strcpy(footpath.segside, array[11]);
    footpath.statusid = atoi(array[12]);
    footpath.streetid = atoi(array[13]);
    footpath.street_group = atoi(array[14]);
    footpath.start_lat = atof(array[15]);
    footpath.start_lon = atof(array[16]);
    footpath.end_lat = atof(array[17]);
    footpath.end_lon = atof(array[18]);
    return(footpath);
}

// Initialises a rectangle
struct rectangle2D* createRectangle(point2D bottomLeft, point2D topRight){
    rectangle2D* newRectangle = (rectangle2D*)malloc(sizeof(rectangle2D));
    newRectangle->bottomLeft = bottomLeft;
    newRectangle->topRight = topRight;
    return(newRectangle);
}

// Returns true if a point falls within a rectangle's bounds, otherwise false
int inRectangle(rectangle2D* rectangle, point2D point){
    if (point.x > rectangle->bottomLeft.x){
        if (point.y > rectangle->bottomLeft.y){
            return(TRUE);
        }
    }
    return(FALSE);
}

// Initialises a quad tree node
struct quadTreeNode* initialiseNode(point2D bottomLeft, point2D topRight){
    struct quadTreeNode* newNode = malloc(sizeof(struct quadTreeNode));
    newNode->rectangle = createRectangle(bottomLeft, topRight);
    newNode->state = WHITE;
    newNode->count = 0;
    return(newNode);
}

// prints the struct details of the footpath. taken from ass.1
void printStruct(struct Footpath footpath, FILE* outputFile){
    fprintf( outputFile, "--> footpath_id: %d || "
    "address: %s || "
    "clue_sa: %s || "
    "asset_type: %s || "
    "deltaz: %f || "
    "distance: %f || "
    "grade1in: %f || "
    "mcc_id: %d || "
    "mccid_int: %d || "
    "rlmax: %f || "
    "rlmin: %f || "
    "segside: %s || "
    "statusid: %d || "
    "streetid: %d || "
    "street_group: %d || "
    "start_lat: %f || "
    "start_lon: %f || "
    "end_lat: %f || "
    "end_lon: %f || "
    "\n",
    footpath.footpath_id, footpath.address, footpath.clue_sa, footpath.asset_type, 
    footpath.deltaz, footpath.distance, footpath.grade1in, footpath.mcc_id, 
    footpath.mccid_int, footpath.rlmax, footpath.rlmin, footpath.segside, 
    footpath.statusid, footpath.streetid, footpath.street_group, footpath.start_lat, 
    footpath.start_lon, footpath.end_lat, footpath.end_lon);
}

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

// concatenates two linked lists
void append(listNode* results, listNode* additionalPoints){
    if (results->next == NULL)
        results->next = additionalPoints;
    else
        append(results->next,additionalPoints);
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

// Search the quad tree to return nodes within a given boundary.
// Inspired by https://en.wikipedia.org/wiki/Quadtree#Query_range
int rangeQuery(listNode* result, quadTreeNode* head, rectangle2D* rectangle, FILE* outputFile){
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
           rangeQuery(result, current->SW, rectangle, outputFile); }
       if (rectangleOverlap(current->NW->rectangle, rectangle) == TRUE){ 
           if (current->NW->state != WHITE){
            printf(" NW");
           }
           rangeQuery(result, current->NW, rectangle, outputFile); }
       if (rectangleOverlap(current->NE->rectangle, rectangle) == TRUE){ 
           if (current->NE->state != WHITE){
            printf(" NE");
           }
           rangeQuery(result, current->NE, rectangle, outputFile); }
       if (rectangleOverlap(current->SE->rectangle, rectangle) == TRUE){ 
           if (current->SE->state != WHITE){
            printf(" SE");
           }
           rangeQuery(result, current->SE, rectangle, outputFile); }
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

// Free a linked list.
void freeList(struct listNode* head){
    struct listNode* current = head;
    while (head != NULL){
        current = head; 
        head = head->next;
        free(current);
    }
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
        node->count = 1;
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


// Split the node and push its data down
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

    // Mark the node as being split already

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

// Gets input file from argv. Taken from assignment 1
FILE* getInputFile(char* file){

    FILE* inputFile;
    inputFile = fopen(file, "r");
    return(inputFile);
}

// creates an output file from argv. Taken from ass.1
FILE* makeOutputFile(char* file){
    FILE* outputFile;
    outputFile = fopen(file, "w+");
    return(outputFile);
}

// Removes commas from field. Taken from ass.1
int removeCommas(char* array, int length){
    int i, j;
    for (i=0; i<length; i++){
        if (array[i] == '"'){
            for (j=i+1; j<length; j++){
                // replace with placeholder
                if (array[j] == ','){
                    array[j] = '|';
                }
                if (array[j] == '"'){
                    return(1);
                }
            }
        }
    }
    return(0);
}

// Edits fields of the CSV to remove quotations. Taken from ass.1
int editField(char field[MAX_LEN], int len){
    int i, j;
    j = 0;
    //int ifComma =0;

    for (i=0; i<len; i++){
        if (field[i] == '"'){
         //   ifComma = 1;
            break;
        }
    }
    if (field == 0){
        return(0);
    }

    j = 0;

    // Adds commas back into field and removes quotation marks 
    for (i=0; i<len; i++){
        if (field[i] == '|'){
            field[j] = ',';
            j++;
        }
        else if (field[i] != '"' || field[i] != '"'){
            field[j] = field[i];
            j++;
        }
    }
    field[j] = '\0';
return(0);

}

// Reads the input file and operates. Inspired from ass.1
struct quadTreeNode* readInputFile(FILE* inputFile, quadTreeNode* root){
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
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
            dataPoint startDataPoint = createDataPoint(&currFootpath, startPoint);
            dataPoint endDataPoint = createDataPoint(&currFootpath, endPoint);

            // Inserts the data to the Quad Tree from the start position and the end position of the CSV record.
            insertIntoQuadTree(root, startDataPoint);
            insertIntoQuadTree(root, endDataPoint);
        }
    }
    
    free(line);
    return(root);
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

// Empty Footpath initialiser to help with memory freeing
struct Footpath createEmptyStruct(){
    Footpath footpath = {};
    return(footpath);
}

// Parse the info given in stdin so it can be read by the functions
void interpret(char ss[MAX_LEN], quadTreeNode* head, FILE* outputFile){
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
    rangeQuery(result, head, currBoundary, outputFile);
    free(currBoundary);

    // Sort matches and remove duplicates before printing
    assert(result != NULL);
    sortList(result);
    removeDuplicates(result);
    printList(result, outputFile);
    freeList(result);
    printf("\n");

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



// Taken from my submission of ass.1 
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
    root->count=0;
    
    readInputFile(inputFile, root);
    read_strings(outputFile, root);

    fclose(outputFile);
    fclose(inputFile);

    free_root(root);

    return(0);
    }
