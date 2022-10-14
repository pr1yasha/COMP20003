#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "quadTree.h"
#include "list.h"
#include "data.h"

#define NE_VAL 0
#define SE_VAL 1
#define SW_VAL 2
#define NW_VAL 3
#define WHITE 0
#define BLACK 1
#define GREY 2

typedef struct quadTreeNode {
    int state;
    struct rectangle2D* rectangle;
    struct dataPoint data;
    struct quadTreeNode* NW;
    struct quadTreeNode* NE;
    struct quadTreeNode* SE;
    struct quadTreeNode* SW;
} quadTreeNode;

int free_root(quadTreeNode* root_node);
void free_node(quadTreeNode* node);
int splitNode(quadTreeNode* node);
void addItemToList(struct quadTreeNode* baseNode, struct Footpath footpath);
struct quadTreeNode* initialiseNode(point2D bottomLeft, point2D topRight);
int insertIntoQuadTree(quadTreeNode* node, dataPoint data);
struct quadTreeNode* readInputFile(FILE* inputFile, quadTreeNode* root);
void read_strings(FILE* outputFile, quadTreeNode* head);
void interpret(char ss[MAX_LEN], quadTreeNode* head, FILE* outputFile);
int searchDict4(listNode* result, quadTreeNode* head, rectangle2D* rectangle, FILE* outputFile);
void readStringsDict4(FILE* outputFile, quadTreeNode* head);

#endif
