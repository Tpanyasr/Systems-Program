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

#define MAXINPUT 4112

int childrenArr[10];  // list of children pids 
int childInd = 0; // keeps track of children 
int pfd[2]; // parents file descriptors
int childPipes[10][2]; // array of children's pipes fd's 
int ppid; // parents proccess id

void signal1(int siginf)
{
    // should change STDIN_FILENO to pfd[0] and print information from child
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
            char period[] = ".";
            searchDir(search,period, i, paths);
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

void killMethod(int childNum) // method to kill children from shell
{
    kill(childrenArr[childNum-1], 2);
}

void quitMethod() // should have the parent kill all children then itself
{
    for(int i = 0; i < 10; i++)
    {
        if(childrenArr[i] != 0)
        {
            killMethod(i+1);
        }
    }
}

void listMethod()
{
    //should list the children and what they do 
}




int main()
{
    
    ppid = getpid();
    pipe(pfd); // parents pipe
    char text[MAXINPUT];
    
    int realstdin = dup(STDIN_FILENO); // saves stdin 

    for(int i = 0; i < 10; i++) // sets up all pipes for potential children to read from parents fd[1] and writes to parents fd[0]
    {
        pipe(childPipes[i]);
        dup2(pfd[1], childPipes[i][0]);
        dup2(pfd[0], childPipes[i][1]);
    }

    while(1)
    {
        fprintf(stderr, "\033[0;34m");
        fprintf(stderr, "findstuff");
        fprintf(stderr,"\033[0m");
        fprintf(stderr, "$ ");
        read(STDIN_FILENO, text, MAXINPUT);
        write(pfd[1], text, MAXINPUT);
        dup2(realstdin, STDIN_FILENO);
        
        if((childrenArr[childInd++] = fork()) == 0)
        {
            char input[MAXINPUT];
            char *args[4];
            int i = 0;
            read(childPipes[childInd-1][0], input, MAXINPUT);
            char* token = strtok(input, " ");
            while(token != NULL)
            {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            if(strcmp(args[0], "find") == 0)
            {
                if(args[2])
                {
                    if(args[2][0] == '-' && args[2][1] == 's')
                    {
                        find(args[1], 1, NULL);
                    }
                    else
                    {
                        if(args[3])
                        {
                            find(args[1], 1, args[2]);
                        }
                        else
                        {
                            find(args[1], 0, args[2]);
                        }
                    }
                }
                else
                {
                    find(args[1], 0, NULL);
                }
            }
            else if(strcmp(args[0], "quit") == 0 || strcmp(args[0], "q") == 0)
            {
                quitMethod();
            }
            else if(strcmp(args[0], "kill") == 0)
            {
                int num = atol(args[1]);
                killMethod(num);
            }
            else if(strcmp(args[0], "list") == 0)
            {
                listMethod();
            }
            else
            {
                perror("Invalid input\n");
                exit(EXIT_FAILURE);
            }
            return 0;
        }
        else
        {
            int status;
            waitpid(childrenArr[childInd-1],&status, WNOHANG);
        }
        
    }
    
    return 0;
}