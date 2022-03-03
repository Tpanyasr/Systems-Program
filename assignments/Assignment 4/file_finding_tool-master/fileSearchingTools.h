#ifndef FILE_SEARCHING_TOOLS_H
#define FILE_SEARCHING_TOOLS_H

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include "dataStructures.h"


int parseForOccurence(fileSearch filesrch, char *filename, char *path);

int searchForFile(fileSearch filesrch, char *path, DIR *dir);

int find(fileSearch filesrch, char *path);

#endif
