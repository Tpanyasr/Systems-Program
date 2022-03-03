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


int numChild = 0;


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

int sub_find_file(char *file, struct dirent *entry, char *path, int match){
    char tempPath[1000];
    DIR *dir;
    dir = opendir(path);
    if(!dir){
        //printf("%s \n", path);
        perror("opendir() failed");
        return -1;
    }
    struct dirent *sentry;
    //int match = 0;
    while((sentry = readdir(dir)) != NULL){
        //printf("%s \n", sentry->d_name);
        if(sentry->d_type == 4 && strcmp(sentry->d_name, ".") != 0 && strcmp(sentry->d_name, "..") != 0){
            //printf("%s \n", file);
            strcpy(tempPath, path);
            strcat(tempPath, "/");
            strcat(tempPath, sentry->d_name);
            //printf("here: %s \n", tempPath);
            match = sub_find_file(file, sentry, tempPath, match);
        }
        if( (strcmp(file, sentry->d_name) == 0) ){
            //printf("here \n");
            printf("%s/%s \n", path, sentry->d_name);
            match = 1;
        }
    }
    closedir(dir);
    return match;
}

int find_file(char *file, char *flag){
    struct timeval start, stop;
    double secs = 0;

    gettimeofday(&start, NULL);


    char path[1000];
    getcwd(path, 1000);
    DIR *dir;
    int match = 0;
    struct dirent *entry;
    
    /* check subdirectories */
    printf("Matching file paths: \n");
    if( (strcmp(flag, "-s") == 0) ){
        dir = opendir(".");
        if(!dir){
            perror("opendir() failed");
            return -1;
        }
        while((entry = readdir(dir)) != NULL){
            //printf("%d \t", entry->d_type);
            if(entry->d_type == 4 && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){
                match = sub_find_file(file, entry, path, match);
            }
        }
        closedir(dir);
    }
    else{
        //printf("here \n");
        dir = opendir(".");
        if(!dir){
            perror("opendir() failed");
            return -1;
        }
        while((entry = readdir(dir)) != NULL){
            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
                continue;
            }
            if( (strcmp(file, entry->d_name) == 0) ){
                printf("%s/%s \n", path, entry->d_name);
                match = 1;
            }
        }
        closedir(dir);
    }

    if(match != 1){
        return -1;
    }
    putchar('\n');

    
    gettimeofday(&stop, NULL);
    secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    printf("time taken %f\n",secs);
    return 0;

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
    //char *usrinput = mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    int * childPIDs = (int *)mmap(NULL, sizeof(int) *10, PROT_READ|PROT_WRITE, MAP_SHARED |MAP_ANONYMOUS, -1, 0);  // list of children pids 
    char ** childtasks = (char **)mmap(NULL, sizeof(char*) *10, PROT_READ|PROT_WRITE, MAP_SHARED |MAP_ANONYMOUS, -1, 0); //list of all child tasks
    int fd[2];
    pipe(fd);
    int save_usrinput = dup(STDIN_FILENO);
    dup2(fd[0], STDIN_FILENO);
    int status;
    int prompt = fork();

    while(1)
    {
        if(prompt == 0)
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
            char input[PATH_MAX];
            char *args[4];
            int i =0;
            close(fd[1]);
            dup2(save_usrinput, STDIN_FILENO);
            read(STDIN_FILENO, input, PATH_MAX);
        
            char* token = strtok(input, " \n");
            while(token != NULL)
            {
                args[i++] = token;
                token = strtok(NULL, " \n");
            }
            if(strcmp(args[0], "q") == 0 || strcmp(args[0], "quit") == 0)
            {
                kill(prompt, SIGKILL);
                break;
            }
            if(strcmp(args[0], "list") == 0)
            {
                printf("Number of active processes: %d \n", numChild);
                for(int i = 0; i < 10; i++){
                    if(childtasks[i] != NULL)
                    {
                        printf("Process %d is finding %s \n", i+1, childtasks[i]);
                    }
                }
            }
            if(strcmp(args[0], "kill") == 0)
            {
                int pidSerial = atoi(args[1]);
                if(childtasks[pidSerial-1] != NULL)
                {
                    kill(childPIDs[pidSerial-1], SIGKILL);
                    numChild-=1;
                    childPIDs[pidSerial-1] = 0;
                    childtasks[pidSerial-1] = NULL;
                }
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
                    
                        if(args[2] !=NULL && (args[2][0] == '-' && args[2][1] == 's'))
                        {
                        
                        }
                        //usage -f:xyz
                        if(args[2] !=NULL && (args[2][0] == '-' && args[2][1] == 'f'))
                        {
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
                                    printf(" You entered -f and -s\n");
                                }
                        }
                        else
                        {
                            find_file(args[1], args[2]);
                        }       
                    }
                    return 0;
                }
                else
                {
                    while(1)
                    {
                    //sleep(1);
                        int ret = waitpid(child_pid, &status, WNOHANG);
                        if(ret > 0)
                        {
                            printf("child %d returned \n", numChild);
                            numChild-=1;
                            childPIDs[pidSerial-1];
                            childtasks[pidSerial-1] = NULL;
                            break;
                        }
                    }
                            kill(child_pid, SIGKILL);
                            //return 0;
                }
            }
            close(fd[0]);
            waitpid(prompt, &status, 0);
            kill(prompt, SIGKILL);
            prompt = fork();
        }
    }
    return 0;
}