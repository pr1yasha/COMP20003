#ifndef _DATA_H_
#define _DATA_H_

#include "quadTree.h"
#include "list.h"
#include "data.h"

#define TRUE 1
#define FALSE 0
#define MAX_LEN 500

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

typedef struct point2D {
    long double x;
    long double y;
} point2D;

typedef struct rectangle2D {
    point2D bottomLeft;
    point2D topRight; 
} rectangle2D;

typedef struct dataPoint {
    struct point2D location;
    struct Footpath* footpath;
    struct listNode* footpathNodes;
} dataPoint;

// Function prototypes
FILE* getInputFile(char* file);
FILE* makeOutputFile(char* file);
void printStruct(struct Footpath footpath, FILE* outputFile);
struct Footpath createStruct(char *array[]);
int removeCommas(char* array, int length);
int editField(char field[MAX_LEN], int len);
struct rectangle2D* createRectangle(point2D bottomLeft, point2D topRight);
int determineQuadrant(rectangle2D* rect, point2D point);
int rectangleOverlap(rectangle2D* rect1, rectangle2D* rect2);
struct dataPoint createDataPoint(struct Footpath* footpath, struct point2D location);
struct rectangle2D* makeRectangle(point2D bottomLeft, point2D topRight);
struct point2D createPoint2D(long double x, long double y);
int inRectangle(rectangle2D* rectangle, point2D point);
struct Footpath createEmptyStruct();

#endif
