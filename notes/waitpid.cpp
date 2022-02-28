#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
using namespace std;

//waitpid(pid, &status, 0|);
typedef unsigned char BYTE;

int main()
{
    int status;
    int pid = fork();
    if(pid==0)
    {
        sleep(2);
        return 0;
    }
    else
    {
        int pid2 = fork();
        if(pid2 == 0)
        {
            sleep(3);
            return 0;

        }
        while(1)
        {
            int ret = waitpid(pid2, &status, WNOHANG);
            if(ret>0) break;
            usleep(100);
            printf("child 2 has not yet returned \n");
        }
        printf("child 2 returned \n");
        waitpid(pid, &status, 0);
        printf("child 1 returned \n");

    }
    return 0;
}
