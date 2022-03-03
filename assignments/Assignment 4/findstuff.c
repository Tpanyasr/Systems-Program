#include <stdio.h>
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

#define MAXINPUT 4112
int numChild = 0; // keeps track of children 


void signal1(int siginf)
{
    // should change STDIN_FILENO to fd[0] and print information from child
}

void signal2(int sig)
{
    // should be used to run the list or quit command so that it can be run in parent whoo has all PID's
}

void searchDir(char* search, char* dirname, int index, char** paths)
{
    DIR* dir = opendir(dirname);
    struct dirent *file;
    struct stat sb;
    while((file = readdir(dir)) != NULL)
    {
        stat(file->d_name, &sb);
        if(S_ISDIR(sb.st_mode))
        {
            searchDir(search, file->d_name, index++, paths);
        }
        if(strcmp(search, file->d_name) == 0)
        {
            paths[index++] = file->d_name;
        }
    }
}

void find(char* search, int s, char* filetype)
{
    time_t start = clock();
    int i = 0;

    if(search[0] == '"') // if looking for a word in a file
    {
        if(s) // if subdirectory search is on run recursive version
        {
            
        }
        else
        {

        }
    }

    else
    {
        char *paths[PATH_MAX];
        
        if(s) // if subdirectory search is on run recursive function
        {
            searchDir(search,".", i, paths);
            i++; 
        }
        else
        {
            
            DIR* dir = opendir(".");
            struct dirent *file;
            while((file = readdir(dir)) != NULL)
            {
                if(strcmp(search, file->d_name) == 0)
                {
                    paths[i++] = file->d_name;
                }
            }
            
        }
        if(i == 0)
        {
            fprintf(stderr, "No file found");
        }
        else
        {
            fprintf(stderr, "Found file specified");
        }

    }

    time_t end = clock();
    float sec;
    int h, m, ss, ms;    
    sec = (end - start)/CLOCKS_PER_SEC;
	h = (sec/3600);	
	m = (sec -(3600*h))/60;
	ss = (sec -(3600*h)-(m*60));
    ms = (sec - (3600*h) - (m*60) - s) * 100;
	printf("H:M:S - %d:%d:%d:%d\n",h,m,ss,ms);
    
	return;
}

void killchild(int num, int * pidlist, char **childtasks) // method to kill children from shell
{
    kill(pidlist[num-1], 2);
    pidlist[num-1] = 0;
    childtasks[num-1] = NULL;
}

void quitAll(int * pidlist, char ** childtasks) // should have the parent kill all children then itself
{
    for(int i = 1; i < 11; i++)
    {
        if(pidlist[i] != 0)
        {
            killchild(i, pidlist, childtasks);
        }
    }
}

void listAll(char **childtasks, int *pidlist)
{
    for(int i = 0; i < 10; i++)
    {
        if(childtasks[i] ==NULL | pidlist[i] == 0) printf("Process %d has not been assigned a task.\n\n", i+1);
        //else
        printf("Process %d is currently finding %s\n\n", i + 1, childtasks[i]);
    }
}




int main()
{

    int * childPIDs = (int *)mmap(NULL, sizeof(int) *10, PROT_READ|PROT_WRITE, MAP_SHARED |MAP_ANONYMOUS, -1, 0);  // list of children pids 
    char ** childtasks = (char **)mmap(NULL, sizeof(int) *10, PROT_READ|PROT_WRITE, MAP_SHARED |MAP_ANONYMOUS, -1, 0); //list of all child tasks
    int fd[2]; // parents file descriptors
    int ppid; // parents proccess id
    int status;
    int child_pid;
    ppid = getpid();
    pipe(fd); // parents pipe
    char text[MAXINPUT];
    
    int realstdin = dup(STDIN_FILENO); // saves stdin 
    dup2(fd[1], STDIN_FILENO);

    while(1)
    {
        if(numChild < 10){
            numChild++;
            child_pid = fork();
        }
        if(childPIDs[numChild-1]==0)
        {
            fprintf(stderr, "\033[0;34m");
            fprintf(stderr, "findstuff");
            fprintf(stderr,"\033[0m");
            fprintf(stderr, "$ ");
            read(STDIN_FILENO, text, MAXINPUT);
            write(fd[0], text, MAXINPUT);

            childPIDs[numChild-1] = getpid(); //-1 because we incremented the child index and indexes start at 0 not 1.
            close(fd[1]);
            char input[MAXINPUT];
            char *args[4];
            int i = 0;
            dup2(realstdin, STDIN_FILENO);




            //FYI strtok is basically strsplit()
            //split the arguments from read by the delimeter (" ") and puts each of them into a seperate index of array args[].
            char* token = strtok(input, " ");
            
            while(token != NULL)
            {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            childtasks[numChild-1] = args[0]; //store what the child is doing
            printf("I am the child!\n");
            printf("%s  %s", childtasks[0], args[0]);
            if(strcmp(args[0], "find") == 0)
            {
                printf("entered\n");
                if(args[2])
                {
                    if(args[2][0] == '-' && args[2][1] == 's')
                    {
                        printf("You entered -s");
                    }
                    if(args[2][0] == '-' && args[2][1] == 'f')
                        {
                            char *file_ending[2];
                            char *token = strtok(input, ":");

                            //extract the file ending into file_ending[1] :)
                            while( token != NULL ) {
                                file_ending[i++] = token;
                                token = strtok(NULL, " ");
                            }
                            if(args[3][1] == 's')
                            {
                                //find 
                            }

                        }
                }
                
            }
            else if(strcmp(args[0], "quit") == 0 || strcmp(args[0], "q") == 0)
            {
               quitAll(childPIDs, childtasks);
               //parent kills itself
               kill(ppid, 2);
               return 0;
            }
            else if(strcmp(args[0], "kill") == 0)
            {
               int num = atol(args[1]);
                killchild(num, childPIDs, childtasks);
            }
            else if(strcmp(args[0], "list") == 0)
            {
               listAll(childtasks, childPIDs);
            }
            return 0;
        }
        else
        {
            int status;
            
            int ret = waitpid(childPIDs[numChild-1],&status, WNOHANG);
            if(ret>0)
            {
                break;
            }
        }
    }   
    
        
    return 0;
}