#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <iostream>


using namespace std;

int fd[2];
int numChild = 0;

void signalhandler(int i)
{
    dup2(fd[0], STDIN_FILENO);
}


int file_search(int recursive, char *target){
    struct timeval start, stop;
    double secs = 0;

    gettimeofday(&start, NULL);
    //how many matching file names were found ...return this value 
    int matches = 0;
    int found =0;
    DIR *d;
    struct dirent *entry;
    d = opendir(".");
    if (d) {
        while ((entry = readdir(d)) != NULL) {
            if(entry->d_type != DT_DIR)
            {
                    if(strcmp(target, entry->d_name)==0){
                        found = 1;
                        char cwd[PATH_MAX];
                        getcwd(cwd, sizeof(cwd));
                        printf("%s found in %s\n", target, cwd);
                        matches++;
                        if(recursive == 0)
                        {
                            gettimeofday(&stop, NULL);
                            secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
                            printf("time taken %f\n",secs);
                            closedir(d);
                        }
                    }
            }
            if(entry->d_type == DT_DIR && strcmp(entry->d_name, "..") && strcmp(entry->d_name,".")){
                if(recursive == 1){
                    char temp[PATH_MAX] = "./";
                    strcat(temp, entry->d_name);
                    chdir(temp);
                    matches += file_search(recursive, target); //recursively file_search child directory
                    chdir("..");
                }
            }
            
        }
    }
    if(recursive == 1 && found == 1)
    {
    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    printf("time taken %f\n",secs);
    closedir(d);
    }
    return matches;
}

void killchild(char * which, int * pidlist, char **childtasks) // method to kill children from shell
{
    int num = atoi(which);
    kill(pidlist[num-1], 2);
    pidlist[num-1] = 0;
    childtasks[num-1] = NULL;
    printf("\n%d was killed.\n", num);
}

void list(char ** childtasks, int *childPIDs)
{
    for(int i = 0; i < 10; i++)
    {
        if(childtasks[i] ==NULL | childPIDs[i] == 0) printf("Process %d has not been assigned a task.\n", i+1);
        else
        printf("Process %d is currently finding %s\n\n", i + 1, childtasks[i]);
    }
}
int main()
{
    int * childPIDs = (int *)mmap(NULL, sizeof(int) *10, PROT_READ|PROT_WRITE, MAP_SHARED |MAP_ANONYMOUS, -1, 0);  // list of children pids 
    char ** childtasks = (char **)mmap(NULL, sizeof(char*) *10, PROT_READ|PROT_WRITE, MAP_SHARED |MAP_ANONYMOUS, -1, 0); //list of all child tasks
    int * usage = (int *)mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    pipe(fd);
    int savestdin = dup(STDIN_FILENO);
    dup2(fd[0], STDIN_FILENO);
    int status;
    *usage = fork();
    while(1)
    {
        if(*usage == 0)
        {
            close(fd[0]);
            printf("\033[0;34m");
            printf("findstuff$ ");
            printf("\033[0;m");
            close(fd[1]);
            return 0;
        }
        else
        {
            
            close(fd[1]);
            dup2(savestdin, STDIN_FILENO);
            char input[PATH_MAX];
            char *args[4];
            int i =0;
            read(STDIN_FILENO, input, PATH_MAX);
        
            char* token = strtok(input, " \n");
            while(token != NULL)
            {
                args[i++] = token;
                token = strtok(NULL, " \n");
            }
            if(strcmp(args[0], "q") == 0 || strcmp(args[0], "quit") == 0)
            {
                kill(*usage, SIGKILL);
                break;
            }
            if(strcmp(args[0], "list") == 0)
            {
              list(childtasks, childPIDs);
            }
            if(strcmp(args[0], "kill") == 0)
            {
                killchild(args[1], childPIDs, childtasks);
            }
            
            if( (strcmp(args[0], "find") == 0) )
            {
                numChild+=1;
                if(numChild > 10)
                {
                    fprintf(stderr, "There cannot be more than 10 processes\n");
                    break;
                }


                int pidSerial = numChild;
                //printf("%d \n", numChild);
                int status;
                int child_pid = fork();
                childPIDs[pidSerial-1] = child_pid;
                childtasks[pidSerial-1] = args[1];
                //printf("Child %d is finding %s \n", childPIDserial, childtasks[childPIDserial-1]);
                if(child_pid == 0)
                {
                    childtasks[numChild-1] = args[0]; //store what the child is doing
                    if(strcmp(args[0], "find") == 0)
                    {
                        if(args[1][0] == '"')
                        {

                            if(args[2] !=NULL && (args[2][0] == '-' && args[2][1] == 'f'))
                            {
                                //was not able to finish this
                                    char *file_ending[2];
                                    char *token = strtok(input, ":");

                                            //extract the file ending into file_ending[1] :)
                                    while( token != NULL ) 
                                    {
                                        file_ending[i++] = token;
                                        token = strtok(NULL, " ");
                                    }
                                    if(args[3][1] == 's')
                                    {
                                        //was not able to finish these
                                        printf("\nYou entered -f:xyz and -s but this has not been implemented sorry!\n\n");
                                    }
                                    if(args[3][1] != 's')
                                    {
                                        printf("\nYou entered -f:xyz but this has not been implemented sorry!\n\n");

                                    }
                                    
                            }

                            if(args[2] !=NULL && (args[2][0] != '-' && args[2][1] != 'f'))
                            {
                                printf("\nYou are file_searching for text but this has not been implemented sorry!\n\n");
                            }
                                //file_search for text
                        }
                        else
                        {
                            if(args[2] !=NULL && (args[2][0] == '-' && args[2][1] == 's'))
                            {
                                if(file_search(1, args[1])==0)
                                {
                                    printf(">nothing found<\n");
                                }
                                else
                                {
                                    kill(numChild, SIGUSR1);

                                    printf("%s was found. \n", args[1]);
                                }
                            }
                            //usage -f:xyz
                            
                            else if(args[2] !=NULL && (args[2][0] != '-' && args[2][1] != 's')
                                    && (args[2][0] != '-' && args[2][1] != 'f'))
                            {
                                if(file_search(0, args[1])==0)
                                {
                                printf(">nothing found<\n");
                                }
                                else
                                {
                                    kill(numChild, SIGUSR1);
                                    printf("%s was found. \n", args[1]);
                                }
                            }
                        }       
                    }
                    else
                    {
                        printf("Invalid input.");
                    }
                    return 0;
                }
                else
                {
                    while(1)
                    {
                        int ret = waitpid(child_pid, &status, WNOHANG);
                        if(ret > 0)
                        {
                            printf("Process %d ended \n", numChild);
                            numChild-=1;
                            childPIDs[pidSerial-1];
                            childtasks[pidSerial-1] = NULL;
                            break;
                        }
                    }
                            kill(child_pid, SIGKILL);
                }
            }
            close(fd[0]);
            int save_stdin = dup(STDIN_FILENO);
            close(fd[1]);
            dup2(save_stdin, STDIN_FILENO);
            signal(SIGUSR1, signalhandler);
            waitpid(*usage, &status, 0);
            *usage = fork();
            
        }
    }
    munmap(childPIDs, sizeof(int)*10);
    munmap(childtasks, sizeof(char)*10);
    return 0;
}