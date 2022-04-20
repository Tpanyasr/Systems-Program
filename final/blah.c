#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

typedef unsigned char BYTE;

int function(int * x)
{
    *x = *x+1;
    // printf("This is the new val! %d\n", x);    return 0;
}
int main()
{
    int x = 4;
    char c;
    function(&x);
    c = 'g';

    printf("%d", x);
    return 0;
}
