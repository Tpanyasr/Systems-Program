#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

int fd[2];

void signalhandler(int i)
{
    dup2(fd[0], STDIN_FILENO);
    printf("overwrite stdin\n");
}

int main()
{

    pipe(fd);

    int parentPID = getpid();

    char text[100];

    int child = fork();
    if(child ==0)
    {
        sleep(2);
        close(fd[0]);
        kill(parentPID, SIGUSR1);
        write(fd[1], "hello", 6);
        close(fd[1]);
        return 0;
    }
    else
    {
       int save_stdin = dup(STDIN_FILENO);
       close(fd[1]);
       signal(SIGUSR1, signalhandler);

       
       //scanf("%s", text);
       read(STDIN_FILENO, text, 20);

       printf("%s\n", text);

       printf("restore stdin\n");

       dup2(save_stdin, STDIN_FILENO);
       text[0]= 0;
       //scanf("%s", text);
       read(STDIN_FILENO, text, 20);
       printf("%s\n", text);

       wait(0);
    }
    close(fd[0]);
    
    return 0;
}
