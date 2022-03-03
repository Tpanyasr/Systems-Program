// Name: Aimee Tan
// Course: CPE 357, Fall 2021
// Assignment: Lab 7

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char BYTE;
typedef struct mypipe
{
    BYTE* pipebuffer;
    int buffersize;
    int start_occupied;
    int end_occupied;
} mypipe;

//initializes (malloc) the pipe with a size of "size" and sets start and end.
void init_pipe(mypipe* pipe, int size)
{
    // initialize attributes of pipe
    pipe->pipebuffer = malloc(size);
    pipe->buffersize = size;
    pipe->start_occupied = pipe->end_occupied = 0;
}

//writes "size" bytes from buffer into the pipe, returns size
int mywrite(mypipe *pipe, BYTE* buffer, int size)
{
    // keep track of how many bytes are written
    int write_count = 0;
    int end = pipe->end_occupied;

    if (size > pipe->buffersize)
    {
        return write_count;
    }

    for (int i = end; i <= (end + size); i++)
    {
        // mod for carry over cases!
        pipe->pipebuffer[i % pipe->buffersize] = buffer[write_count];
        write_count += 1;
    }
    
    // add size end to update position, mod for carry over cases!
    pipe->end_occupied = (pipe->end_occupied + size) % pipe->buffersize;
    return write_count;
}

//reads "size" bytes from pipe into buffer, returns how much it read (max size), 0 if pipe is empty
int myread(mypipe* pipe, BYTE* buffer, int size)
{
    // keep track of how many bytes are read
    int read_count = 0;
    int start = pipe->start_occupied;

    // empty pipe or read more than available in pipe
    if (size > pipe->buffersize || pipe->start_occupied == pipe->end_occupied)
    {
        return read_count;
    }

    for (int i = start; i < (start + size); i++)
    {
        // mod for carry over cases!
        buffer[read_count] = pipe->pipebuffer[i % pipe->buffersize];
        read_count += 1;
    }
    
    // add size to start to update position, mod for carry over cases!
    pipe->start_occupied = (pipe->start_occupied + size) % pipe->buffersize;
    return read_count;
}

int main()
{
    char text[100];
    mypipe pipeA;
    init_pipe(&pipeA, 32);
    mywrite(&pipeA, "hello world", 12);
    mywrite(&pipeA, "it's a nice day", 16);
    myread(&pipeA, text, 12);
    printf("%s\n", text);
    myread(&pipeA, text, 16);
    printf("%s\n", text);
    
    mywrite(&pipeA, "and now we test the carryover", 30);
    myread(&pipeA, text, 30);
    printf("%s\n", text);

    return 0;
}