/*
    Child Process
    scanf the user input
   
   read a filename form the keyboard then print the files info
   "list" lists the content of the current directory 
                -make sure to check for opendir, readdir, and closedir
    "q" ends the complete program:
            - all processes for monitor 1 and only the kid process in monitor 2 

    make sure to print 
    (name of program)<- in blue and $ <- in black text color
    allocate 100 bytes for user input

*/
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <sys/wait.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
using namespace std;

typedef unsigned char BYTE;
int showstat(char *filepath);

void printcwd();

void overwritesig(int i);


int main()
{
    signal(20, overwritesig);
    signal(SIGSTOP, overwritesig);
    signal(SIGINT, overwritesig);
    signal(SIGHUP, overwritesig);
    signal(SIGQUIT, overwritesig);
    signal(SIGTERM, overwritesig);
    DIR *currDir;
    char * boolean = (char *)mmap(NULL,sizeof(int), PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, -1, 0);
    *boolean = 0;
    int ppid = getpid();
    char list[5] = "list";
    char q[2] = "q";

    
    int childPID;
    int parentPID = getpid();
    
    while(1)
    {
    childPID = fork();
    if(childPID == 0)
    {
        while(1)
        {
            *boolean = 1;
            char command[255];
            printf("\033[0;34m");
            char workdir[1000];
            getcwd(workdir, 1000);
            printf("stat prog .");
            cout<<workdir;
            printf("\033[0m");
            printf("$ ");
            int waitSecond = 10;
            scanf("%s", command);
            *boolean = 0;
            if(strcmp(command, list)==0)
            {
                printcwd();

            }
            else if(strcmp(command, q) == 0)
            {

                munmap(boolean, sizeof(int));
                return 0;
            }
            else
            {
                int worked = showstat(command);

            }
            //if a command was entered boolean is true
            
    
        }
    }
    wait(0);
    sleep(10);
    }
        //check if user entered a command 0 is true 1 is false
        
    return 0;
}
   


void printcwd()
{

    char workdir[1000];
    getcwd(workdir, 1000);
    
    DIR *dir;
    struct dirent *entry;
    dir = opendir(".");
    if(dir == NULL)
    {
        cout << "bad case" << endl;
        return;
    }

    while((entry = readdir(dir))!= NULL)
    {
        cout << "___________________________________________________"<<endl;

        cout << entry->d_name << endl;
       

    }
    closedir(dir);
    return;
}


//returns -1 if there is no filename does not exist to stat
int showstat(char *filepath)
{
    
    struct stat sb;
    int ret = stat(filepath, &sb);
    if(ret == -1)
    {
        perror("Filename");
        return -1;
    }
     printf("File type:                ");

           switch (sb.st_mode & S_IFMT) {
           case S_IFBLK:  printf("block device\n");            break;
           case S_IFCHR:  printf("character device\n");        break;
           case S_IFDIR:  printf("directory\n");               break;
           case S_IFIFO:  printf("FIFO/pipe\n");               break;
           case S_IFLNK:  printf("symlink\n");                 break;
           case S_IFREG:  printf("regular file\n");            break;
           case S_IFSOCK: printf("socket\n");                  break;
           default:       printf("unknown?\n");                break;
           }
               printf("I-node number:            %ju\n", (uintmax_t) sb.st_ino);

           printf("Mode:                     %jo (octal)\n",
                   (uintmax_t) sb.st_mode);

           printf("Link count:               %ju\n", (uintmax_t) sb.st_nlink);
           printf("Ownership:                UID=%ju   GID=%ju\n",
                   (uintmax_t) sb.st_uid, (uintmax_t) sb.st_gid);

           printf("Preferred I/O block size: %jd bytes\n",
                   (intmax_t) sb.st_blksize);
           printf("File size:                %jd bytes\n",
                   (intmax_t) sb.st_size);
           printf("Blocks allocated:         %jd\n",
                   (intmax_t) sb.st_blocks);

           printf("Last status change:       %s", ctime(&sb.st_ctime));
           printf("Last file access:         %s", ctime(&sb.st_atime));
           printf("Last file modification:   %s", ctime(&sb.st_mtime));
           return 0;
}

