#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

typedef struct fileSearch{
    char *filename;
    char *report;
    char *filetag;
    int subFlag;
    int textFlag;
    int endFlag;
}fileSearch;

typedef struct childStruct{
    int occupied;
    int pid;
    char command[1000];
}childStruct;

#endif
