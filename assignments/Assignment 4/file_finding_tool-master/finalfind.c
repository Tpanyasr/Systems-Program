#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "dataStructures.h"
#include "fileSearchingTools.h"

int fd[2];

void add_null_term(char *txt){
    for(int i=0;i<100;i++)
        if(txt[i]=='\n') {txt[i+1]=0;break;}
}


void cleanUp(char *line, char *result){
    //firstly: remove all spaces at the front
	char temp[1000];
	int start_space = 1;
	for (int i = 0, u = 0; i <= strlen(line); i++)
		if (line[i] == ' ' && start_space == 1) 
            continue;
		else 
			{
			temp[u++] = line[i]; 
			start_space = 0;
			}
	//now remove an double or triple spaces
	int space_on = 0;
	for (int i = 0, u = 0; i <= strlen(temp); i++)
		{
		if (space_on == 1 && temp[i] == ' ') 
            continue;
		result[u++] = temp[i];
		if (temp[i] == ' ') 
            space_on = 1;
		else 
            space_on = 0;
		}
}


int get_argument(char* line, int argn, char* result)
	{
    char temp[1000];
	cleanUp(line, temp);
	//finally extract the arguments
	int start, end;
	start = end = 0;
	int count = 0;
	int quote = 0;
	for (int i = 0; i <= strlen(temp); i++)
		if (temp[i] == '\"') 
            quote = !quote;
		else if (quote == 0 &&(temp[i] == ' ' || temp[i] == 0))
			{
			end = i;
			if (argn == count)
				{
				int length = end - start;
				strncpy(result, temp + start, length);
				result[length] = 0;
				return 1;
				}
			start = end + 1;
			count++;
			}
	return 0;
}

int getNumArgs(char *line){
    int i, ytSpace = 1, argn = 0;

    for(i = 0; i < strlen(line); i++){
        if(line[i] == '\0' || line[i] == '\n')
            return argn;
        if (line[i] == ' '){
            ytSpace = 1;
            continue;
        }
        else if(line[i] != ' ' && ytSpace == 1){
            ytSpace = 0;
            argn++;
        }        
    }
    return argn;
}

int overridemode=0;

void myfct(int y){
    dup2(fd[0],STDIN_FILENO); //Overwrite userinput
    overridemode=1;
}


/*int parseForOccurence(fileSearch filesrch, char *filename, char *path){
    char fullFilePath[1000];
    FILE *file;
    int size;
    char *content, word[1000], *p;

    strcpy(word, filesrch.filename + 1);

    if (word[strlen(word) - 1] == '\"')
        word[strlen(word) - 1] = 0;

    sprintf(fullFilePath, "%s", path);
    strcat(fullFilePath, "/");
    strcat(fullFilePath, filename);

    file = fopen(fullFilePath, "rb");

    if (file == NULL)
        return 0;

    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    if (size == EOF)
        return 0;
    fseek(file, 0L, SEEK_SET);
    content = (char *)malloc(size);
    fread(content,size,1,file);
    int len = strlen(content);
    if(content[len-1] == '\n')
        content[len-1] = '\0';
    p = strstr(content, word);
    if(p){
        free(content);
        return 1;
    }
    return 0;
}

int searchForFile(fileSearch filesrch, char *path, DIR *dir){
    struct stat path_stat;
    struct dirent *entry;
    int bool1, bool2, bool3, i;
    while ((entry = readdir(dir)) != NULL) {
        stat(entry->d_name, &path_stat);
        bool1 = 1;
        bool2 = 1;
        if (filesrch.endFlag){
            for(i = 0; i < strlen(entry->d_name); i++){
                if(entry->d_name[i] == '.')
                    break;
            }
            if (strncmp(&(entry->d_name[i + 1]), filesrch.filetag, strlen(filesrch.filetag)))
                bool1 = 0;
        }
        if (filesrch.textFlag)
            bool2 = parseForOccurence(filesrch, entry->d_name, path);
        else
            bool2 = !strcmp(entry->d_name, filesrch.filename);
        if (bool1 && bool2){
            strcat(filesrch.report, filesrch.filename);
            strcat(filesrch.report," in ");
            strcat(filesrch.report, path);
            strcat(filesrch.report, "/");
            if (filesrch.textFlag)
                strcat(filesrch.report, entry->d_name);
            strcat(filesrch.report, "\n");
            return 2;
        }     
    }
    return 0;    
}

int find(fileSearch filesrch, char *path){

    char temppath[1000];
    DIR *d = opendir(path);
    struct stat path_stat;
    struct dirent *entry;
    int found = 0;
    strcpy(temppath, path);

    if (!d)
        return 1;
    if (d)
        if (searchForFile(filesrch, path, d) == 2)
            found = 2;
    closedir(d);
    if (filesrch.subFlag){
        d = opendir(path);
        if(d){
            while ((entry = readdir(d)) != NULL) {
                stat(entry->d_name, &path_stat);
                if (strcmp(entry->d_name, ".") 
                && strcmp(entry->d_name, "..")){
                    strcat(temppath, "/");
                    strcat(temppath, entry->d_name);
                    if (find(filesrch, temppath) == 2)
                        found = 2;
                    else
                        strcpy(temppath, path);
                }
            }
        }
    }
    return found;
}*/


int main()
    {
  
    childStruct *childpids = (childStruct *)mmap(0,sizeof(childStruct)*10,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    char input[1000], arg3[10], arg4[10],
    *final = (char *)mmap(0, 1000, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0), 
    *filetag = (char *)mmap(0, 100,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0),
    *filename = (char *)mmap(0, 1000, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
    int parentPid = getpid(), argc,
    *subFlag = (int *)mmap(0,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0), 
    *textFlag = (int *)mmap(0,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0), 
    *endFlag = (int *)mmap(0,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0);
    
    for(int i=0;i<10;i++) 
        childpids[i].occupied = 0;

    signal(SIGUSR1,myfct);
    pipe(fd);
    int save_stdin = dup(STDIN_FILENO), kidnum, status;

    while (1){

        printf("my prog$ ");
        fflush(0);
        dup2(save_stdin,STDIN_FILENO);
        overridemode=0;
        read(STDIN_FILENO,input,1000);

        argc = getNumArgs(input);
        
        cleanUp(input, final);

        if (argc > 4){
            printf("invalid number of arguments\n");
            return 1;
        }

        if(overridemode==0)
            add_null_term(input);//to get a NULL at the end of the string in case of a user input

        if (!strncmp(final, "list", 4)){
            for(int i=0;i<10;i++){ 
                if(childpids[i].occupied){
                    printf("kid #%d:\n\n", i);
                    printf("pid: %d\n", childpids[i].pid);
                    printf("command: %s\n\n", childpids[i].command);
                }
            }
        }else if (!strncmp(final, "kill", 4)){
            if (argc == 1){
                printf("usage: kill [process number]\n");
                printf("enter list to see all the processes currently running\n");
            }else{
                sscanf(final + 5, "%d", &kidnum);
                if (kidnum > 9 || kidnum < 0)
                    printf("there can only be 10 processes running at a time, so please enter a number from 0-9\n");
                else{
                    if(childpids[kidnum].occupied){
                        kill(childpids[kidnum].pid, SIGKILL);
                        waitpid(childpids[kidnum].pid, &status, WNOHANG);
                        childpids[kidnum].occupied = 0;
                    }else{
                        printf("this process has already finished or does not exist.\n");
                        printf("enter list to see all the processes currently running\n");
                    }
                }

            }  
        }else if (!strncmp(final, "q", 1)){
            for (int i =0; i < 10; i++){
                if (childpids[i].occupied){
                    kill(childpids[kidnum].pid, SIGKILL);
                    waitpid(childpids[kidnum].pid, &status, WNOHANG);
                    childpids[kidnum].occupied = 0;
                }
            }
            return 0;
        }else if (strncmp(input,"find",4) == 0){

            get_argument(input, 1, filename);
            if(!strncmp(filename, "\"", 1))
                *textFlag = 1;
            
            if (argc >= 3){
                get_argument(input, 2, arg3);
                if(!strncmp(arg3, "-s", 2))
                    *subFlag = 1;
                else if(!strncmp(arg3, "-f:", 3)){
                    *endFlag = 1;
                    strncpy(filetag, arg3 + 3, 100);
                }
            }

            if (argc == 4){
                get_argument(input, 3, arg4);
                if(!strncmp(arg4, "-s", 2))
                    *subFlag = 1;
                else if(!strncmp(arg4, "-f:", 3)){
                    *endFlag = 1;
                    strncpy(filetag, arg4 + 3, 100);
                }
            }
            


            int sleepcount = 1; //ASCII conversion
            if (fork() == 0) //child process
                {
                char childreport[10000];
                char childFileName[1000];
                char childFileTag[100];
                char time[100];
                long size;
                char *buf;
                char *path;
                fileSearch filesrch;
                //search for an empty spot in the child list
                int kidnum=0;
                for(int i=0;i<10;i++){
                    if(childpids[i].occupied==0){
                        childpids[i].pid=getpid();
                        childpids[i].occupied = 1;
                        kidnum=i;
                        break;
                    }
                    if(i == 9){
                        printf("\ntoo many processes\n");
                        return 1;
                    }
                }
                //printf("kid %d sleeps for %d seconds to indicate a search\n",kidnum,sleepcount)
                //printf("kid %d sleeps for %d seconds to indicate a search\n",kidnum,sleepcount);
                sleep(sleepcount);
                //finding stuff here...
                //finding done.
                size = pathconf(".", _PC_PATH_MAX);
                if ((buf = (char *)malloc((size_t)size)) != NULL)
                    path = getcwd(buf, (size_t)size);
                strcpy(childFileName, filename);
                strcpy(childFileTag, filetag);
                if (childFileName[strlen(filename) - 1] == '\n')
                    childFileName[strlen(filename) - 1] = 0;
                if (childFileTag[strlen(filetag) - 1] == '\n')
                    childFileTag[strlen(filetag) - 1] = 0;
                filesrch.subFlag = *subFlag;
                filesrch.endFlag = *endFlag;
                filesrch.textFlag = *textFlag;
                filesrch.filename = childFileName;
                filesrch.report = childreport;
                filesrch.filetag = childFileTag;                    
                close(fd[0]); //close read            
                sprintf(childreport,"\nkid %d is reporting!",kidnum);
                strcat(childreport,"\nfound stuff:\n");
                struct timeval start, end;
                gettimeofday(&start, NULL);
                if(!find(filesrch, path))
                    strcat(childreport, "\nnothing :(\n");
                gettimeofday(&end, NULL);
	            long seconds = (end.tv_sec - start.tv_sec);
	            long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
                int hours = floor(seconds/3600);
                int minutes = seconds % 60;
                if (*subFlag){
                    sprintf(time, "time taken: %d:%d:%d:%d\n", hours, minutes, seconds, micros);
                    strcat(childreport, time);
                }
                strcat(childreport,"\0");//null terminator is important, because pipe!
                //  printf("%d\n",strlen(childreport));
                int length = strlen(filesrch.report);
                write(fd[1],filesrch.report, length);
                close(fd[1]); //close write  
                kill(parentPid,SIGUSR1);
                childpids[kidnum].occupied = 0;
               
                return 0;
                }
           
            }
        //killing the kid for good:
        for(int i=0;i<10;i++) 
            if(childpids[i].occupied!=0)       
                waitpid(childpids[i].pid,0,WNOHANG);
       
    }
    return 0;

}