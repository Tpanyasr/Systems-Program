#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>

using namespace std;
int search(char *target){
    long ans = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if(dir->d_type != DT_DIR){
                    if(!strcmp(target, dir->d_name)){
                        char cwd[PATH_MAX];
                        getcwd(cwd, sizeof(cwd));
                        printf("%s found in %s\n", target, cwd);
                        ans++;
                    }
            }
            
        }
    }
    closedir(d);
    return ans;
}

int main()
{

    int d;
    if(!d)
    {
        printf("hi");
    }
    return 0;
}
