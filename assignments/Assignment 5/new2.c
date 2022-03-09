
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char* argv[])
{
    int programs = atoi(argv[2]);
    
    for(int i = 0; i < programs; i++)
    {
        char arg1[10];
        char arg2[10];
        sprintf(arg1, "%d", i);
        sprintf(arg2, "%d", programs);
        char * args_input[] = {argv[1], arg1, arg2, NULL};
        if(fork() == 0)
        {
            execv(argv[1], args_input);
            return 0;
        }
        
    }
    wait(NULL);
    return 0;
}