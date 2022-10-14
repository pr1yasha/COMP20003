#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "list.h"

// Creates the footpath struct 
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

// creates an input file from argv
FILE* getInputFile(char* file){
    FILE* inputFile;
    inputFile = fopen(file, "r");
    return(inputFile);
}

// creates an output file from argv
FILE* makeOutputFile(char* file){
    FILE* outputFile;
    outputFile = fopen(file, "w+");
    return(outputFile);
}

// prints the struct details of the footpath
void printStruct(struct Footpath footpath, FILE* outputFile){
    fprintf(outputFile, "--> footpath_id: %d || "
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

// Edits fields of the CSV to remove quotations
int editField(char field[MAX_LEN], int len){
    int i, j;
    j = 0;
    int ifComma =0;

    for (i=0; i<len; i++){
        if (field[i] == '"'){
            ifComma = 1;
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

// Code to remove commas within quotations so they don't get 
// parsed as separate cells.
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
