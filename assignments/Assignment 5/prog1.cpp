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
#include <fcntl.h>

using namespace std;

typedef unsigned char BYTE;

int main()
{
    int fd = shm_open("ournamesharedmem", O_RDWR|O_CREAT, 0777);
    ftruncate(fd, 100*sizeof(int));
    int *p = (int *)mmap(NULL, 100*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    p[0] = 999;
    close(fd);
    shm_unlink("ournamesharedmem");
    munmap(p, 100*sizeof(int));
    return 0;
}
