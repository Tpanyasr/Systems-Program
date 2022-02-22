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

void handler(int i)
{
    return;
}


int main(int argc, char* argv[])
{
    int *used = (int *)mmap(NULL,sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    *used = 0;

    struct sigaction sa = {0};
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = &handler;
    sigaction(2,&sa,NULL);
    sigaction(3,&sa,NULL);
    sigaction(6,&sa,NULL);
    sigaction(9,&sa,NULL);
    sigaction(15,&sa,NULL);
    sigaction(18,&sa,NULL);
    sigaction(19,&sa,NULL);
    sigaction(20,&sa,NULL);

    DIR *currDir;
    int pid;
    int ppid = getpid();
    pid = fork();
    
    
    
    
    while(1)
    {
        
        if(pid != 0)
        {
            sleep(10);
            if(*(used) == 0)
            {
                kill(pid,9);
                wait(NULL);
                *used = 0;
                munmap(used, sizeof(int));
                return 0;
            }
        }
        else
        {
            char input[PATH_MAX];
            printf("\033[0;34m");
            printf("My Sick Prog3");
            printf("\033[0m");
            printf("$ ");
            int scan = scanf("%s", input);
            char ls[5] = "list";
            char *quit = "q";
            if(strcmp(input,ls) == 0)
            {
                currDir = opendir(".");
                struct dirent *file;
                while((file = readdir(currDir)) != NULL)
                {
                    printf("%s\n", file->d_name);
                }
                *(used) = 1;
            }
            else if(strcmp(input,quit) == 0)
            {
                *(used) = 0;
                munmap(used,sizeof(int));
                kill(ppid,9);
                return 0;
            }
            else // statting file and printing stats
            {
                struct stat sb;
                int statRet = stat(input, &sb);
                if(statRet == -1)
                {
                    fprintf(stderr, "Unable to stat: %s\n", input);
                }
                else 
                {
                    fprintf(stderr,"I-node number:                  %ld\n",(long)sb.st_ino);
                    fprintf(stderr,"Mode:                           %lo (octal)\n",(unsigned long)sb.st_mode);
                    fprintf(stderr,"Link Count:                     %ld\n",(long)sb.st_nlink);
                    fprintf(stderr,"Ownership:                      UID=%ld   GID=%ld\n",(long)sb.st_uid,(long)sb.st_gid);
                    fprintf(stderr,"Preferred I/O Block Size:       %ld bytes\n",(long)sb.st_blksize);
                    fprintf(stderr,"File Size:                      %lld\n",(long long)sb.st_size);
                    fprintf(stderr,"Blocks Allocated:               %lld\n",(long long)sb.st_blocks);
                    fprintf(stderr,"Last Status Change:             %s\n",ctime(&sb.st_ctime));
                    fprintf(stderr,"Last File Access:               %s\n",ctime(&sb.st_atime));
                    fprintf(stderr,"Last File Modification:         %s\n",ctime(&sb.st_mtime));
                }
                *(used) = 1;
        
            }    
        }  
    }
    return 0;

}