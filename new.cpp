#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>

using namespace std;
int main()
{

    char * args[4];
    char input[PATH_MAX];
    int i = 0;
    read(STDIN_FILENO, input, PATH_MAX);
    char* token = strtok(input, " \n");
            while(token != NULL)
            {
                args[i++] = token;
                token = strtok(NULL, " \n");
            }
    if(strcmp(args[0], "quit")==0)
    {
        printf("wtf");
    }
    return 0;
}
