#include <stdio.h>
#include <string.h>
#include <iostream>
#define STEM 3
#define SPACES 10
using namespace std;

void printLeaf(int leaf)
{
   
    for(int i = 0; i < leaf; i++){
        printf("*");
    }
    printf("\n");
}

int writeLeaf(int leaf, FILE *file, char *arr)
{
    int size = 0;
   
    for(int i = 0; i < leaf; i++){
        strcat(arr, "*");
        size = size + sizeof('*');
        if (i == leaf-1){
            strcat(arr, "\n");
            size = size + sizeof('\n');
        }
    }
    return size;
    
    
}

int writeTree(int leaves, FILE *file, char *arr){
int spaces = SPACES;
int leafvar = 1;
int size = 0;
//print leaves
for(int i = 1; i < leaves+1; i++)
{

    for(int j = spaces; j > 0; j--)
    {
        strcat(arr, " ");
        size = size + sizeof(' ');
    }
    spaces--;
    size = size + writeLeaf(leafvar, file, arr);
    char current= *(arr + size);
    leafvar = leafvar + 2;
    
}
//"          *\n         ***\n      "

//print stem
for(int k = 0; k < STEM; k++)
{
    strcat(arr, "          *\n");
    size = size + 12;
}
//account for the endl character \0
return size;

}

void printTree(int leaves){

int spaces = SPACES;
int leafvar = 1;
//print leaves
for(int i = 1; i < leaves+1; i++)
{
    for(int j = spaces; j > 0; j--)
    {
        printf(" ");
    }
    spaces--;
    printLeaf(leafvar);
    leafvar = leafvar + 2;
}

//print stem
for(int k = 0; k < STEM; k++)
{
    printf("          *\n");

}

}

int main(int argc, char *argv[])
{
    int input;
    int arrsize = 0;
    char arr[1000];
    *arr = '\0';

    if (argc == 1){
        printf("Enter number of leaves: ");
        scanf("%d", &input);
        printTree(input);
    }
    if (argc == 2){
        int input = atoi(argv[1]);
        if(input > 10){
            input = 10;
        }
        printTree(input);
        
    }
    if(argc == 3){
        int input = atoi(argv[1]);
        if(input > 10){
            input = 10;
        }
        FILE *file = fopen(argv[2],"w");
            if(!file)
            {
                perror("fopen");
            }
        
        arrsize = writeTree(input, file, arr);
    
        fwrite(arr, arrsize, 1, file);
        fclose(file);

    }
    return 0;
}