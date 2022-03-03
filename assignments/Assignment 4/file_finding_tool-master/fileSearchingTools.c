#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include "dataStructures.h"


int parseForOccurence(fileSearch filesrch, char *filename, char *path){
    char fullFilePath[1000];
    FILE *file;
    int size;
    char *content, word[1000], *p;

    strcpy(word, filesrch.filename + 1);

    if (word[strlen(word) - 1] == '\"')
        word[strlen(word) - 1] = 0;

    sprintf(fullFilePath, "%s", path);
    strcat(fullFilePath, "/");
    strcat(fullFilePath, filename);

    file = fopen(fullFilePath, "rb");

    if (file == NULL)
        return 0;

    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    if (size == EOF)
        return 0;
    fseek(file, 0L, SEEK_SET);
    content = (char *)malloc(size);
    fread(content,size,1,file);
    int len = strlen(content);
    if(content[len-1] == '\n')
        content[len-1] = '\0';
    p = strstr(content, word);
    if(p){
        free(content);
        return 1;
    }
    return 0;
}

int searchForFile(fileSearch filesrch, char *path, DIR *dir){
    struct stat path_stat;
    struct dirent *entry;
    int bool1, bool2, bool3, i;
    while ((entry = readdir(dir)) != NULL) {
        stat(entry->d_name, &path_stat);
        bool1 = 1;
        bool2 = 1;
        if (filesrch.endFlag){
            for(i = 0; i < strlen(entry->d_name); i++){
                if(entry->d_name[i] == '.')
                    break;
            }
            if (strncmp(&(entry->d_name[i + 1]), filesrch.filetag, strlen(filesrch.filetag)))
                bool1 = 0;
        }
        if (filesrch.textFlag)
            bool2 = parseForOccurence(filesrch, entry->d_name, path);
        else
            bool2 = !strcmp(entry->d_name, filesrch.filename);
        if (bool1 && bool2){
            strcat(filesrch.report, filesrch.filename);
            strcat(filesrch.report," in ");
            strcat(filesrch.report, path);
            strcat(filesrch.report, "/");
            if (filesrch.textFlag)
                strcat(filesrch.report, entry->d_name);
            strcat(filesrch.report, "\n");
            return 2;
        }     
    }
    return 0;    
}

int find(fileSearch filesrch, char *path){

    char temppath[1000];
    DIR *d = opendir(path);
    struct stat path_stat;
    struct dirent *entry;
    int found = 0;
    strcpy(temppath, path);

    if (!d)
        return 1;
    if (d)
        if (searchForFile(filesrch, path, d) == 2)
            found = 2;
    closedir(d);
    if (filesrch.subFlag){
        d = opendir(path);
        if(d){
            while ((entry = readdir(d)) != NULL) {
                stat(entry->d_name, &path_stat);
                if (strcmp(entry->d_name, ".") 
                && strcmp(entry->d_name, "..")){
                    strcat(temppath, "/");
                    strcat(temppath, entry->d_name);
                    if (find(filesrch, temppath) == 2)
                        found = 2;
                    else
                        strcpy(temppath, path);
                }
            }
        }
    }
    return found;
}
