#ifndef _DATA_H_
#define _DATA_H_

#define MAX_LEN 512

// Struct definitions
struct Footpath {
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
};

// Function prototypes
struct Footpath createStruct(char *array[]);
FILE* getInputFile(char* file);
FILE* makeOutputFile(char* file);
void printStruct(struct Footpath footpath, FILE* outputFile);
int editField(char clue_sa[MAX_LEN], int len);
int removeCommas(char* array, int length);

#endif
