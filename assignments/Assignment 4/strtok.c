#include<stdio.h>
#include <string.h>
#include <sys/mman.h>


int main() {
   char string[50] = "-f:c";
   // Extract the first token
   char *file_ending[2];
   char * token = strtok(string, ":");
   int i= 0;
   // loop through the string to extract all other tokens
   while( token != NULL ) {
      file_ending[i++] = token;
      token = strtok(NULL, " ");
   }

   char ** childtasks = (char **)mmap(NULL, sizeof(int) *10, PROT_READ|PROT_WRITE, MAP_SHARED |MAP_ANONYMOUS, -1, 0);
   childtasks[0] = "hello";
   for(int i = 0; i < 10; i++)
   {
       printf("%s", childtasks[i]);
   }



   printf("%s",file_ending[1]);
   return 0;
}