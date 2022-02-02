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
void showstat (char *filepath);

time_t T = time(NULL);

struct tm tm = *localtime(&T);

typedef unsigned char BYTE;


void printdir(){
    char workdir[1000];
    getcwd(workdir,1000);
    cout <<"current directory: " <<workdir<<endl;

    DIR *dir;
    struct dirent *entry;
    struct stat sb;
    dir = opendir(".");
    if(dir == NULL)
    {
    for(entry = readdir(dir); entry!= NULL; entry = readdir(dir))
    {
        cout <<"______________________________________________________________"<<endl;
        cout <<entry->d_name<<endl;
        showstat(entry->d_name);
    }
    }

}


void overwritesig(int i)
{
    cout << "\nnice try\n" <<endl;
}
int main()
{
    signal(SIGINT, overwritesig);
    signal(SIGHUP, overwritesig);
    signal(SIGQUIT, overwritesig);
    signal(SIGTERM, overwritesig);

    int pidkid = fork();
    cout <<pidkid<<endl;

    struct stat statstruct;

    if(pidkid == 0){
        for(;;)
        {
            sleep(10);
            printf("Current time %d:%d", tm.tm_hour, tm.tm_min);
            printdir();
           
        }
        return 0;
    }
    if(wait(0)){
        cout << getpid() <<endl;
        for(;;)
        {
            sleep(10);
            cout <<tm.tm_hour<<endl;
            cout <<tm.tm_min<<endl;
            printdir();
        }
        return 0;
    }
    return 0;
}



void showstat(char *filepath)
{
           struct stat sb;

           stat(filepath, &sb);
         
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
}

//kill pid to kill  program
//kill -kill pid to MEGA kill 
