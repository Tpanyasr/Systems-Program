#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

typedef unsigned char BYTE;

int main()
{

    int fd[2];
    pipe(fd);

    FILE * file = fopen("test.c", "r");
    FILE *g = fdopen(5, "r");

    char text [100];
    if(fork() ==0)
    {
        close(fd[0]);
        write(fd[1], "hello world", 12);
        write(fd[1], "the world greets you back", 26);
        close(fd[1]);
        return 0;
    }
    else
    {
        close(fd[1]);
        read(fd[0], text, 12);
        printf("%s\n", text);
        read(fd[0], text, 26);
        close(fd[0]);
        printf("%s\n", text);

        wait(0);
    }
    
    return 0;
}
