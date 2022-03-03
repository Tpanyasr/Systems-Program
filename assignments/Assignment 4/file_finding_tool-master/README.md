# FileSystem Search

## Project Overview

This program finds the path of a file in any current working directory as well as its subdirectories by utilizing a concurrent programming design and implementation so that the user can run many simultaneous finding processes. 

## Motivations and Goals

This project was my final project for my systems programming class. It applies all the different concepts that I learned in the class, including how to use dup2(), pipe(), and signals. 

## Challenges

### Understanding New Concepts
To complete this project, I had to have a solid grip on how to establish a pipeline between the child and the parent processes. I also had to figure out how to use `<dirent.h>` and `<sys/stat.h>` so that I could parse specific files.

**My Approach**

I extensively researched all capabilities of `pipe()`, `signal()`, `kill()`, `<dirent.h>`, and `<stat/sys.h>`, and I parsed out what functionalities I thought I needed for the program. I then wrote procedures on how to use all these functionalities on sticky notes for my own reference.

This snippet of code from find.c demonstrates how I used a pipe in the program. (This is code in my child process, both the parent and the child have a copy of the pipe)

```
close(fd[0]); //close read
.
.
.
strcat(childreport,"\0");//null terminator is important, because pipe!
int length = strlen(filesrch.report);
write(fd[1],filesrch.report, length);
close(fd[1]); //close write
kill(parentPid,SIGUSR1);
```

On the parent's side, there is an interrupt function that overrides stdin:

```
void myfct(int y){
    dup2(fd[0],STDIN_FILENO); //Overwrite userinput
    overridemode=1;
}
```

## Project Learnings
Through this project, I established my foundations in concurrent programming, including how to pipe between a child and parent, how to use signals to override stdin, and how redirect the 'read' end of a pipe to stdout using dup and dup2. I have applied this knowledge to other parallelization projects, like matrix multiplication parallelization, which can be seen [here](https://github.com/krishnakalakkad/parallel_matrix_multiplication).



