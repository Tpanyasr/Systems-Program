#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
using namespace std;



typedef unsigned char BYTE;
typedef struct mystruct
{
    BYTE b;
    int i;
}mystruct;

int main()
{

    
    // mystruct s;
    // ifstream file("test.bin"); //input file stream THIS IS A CLASS
    // if(!file.is_open()){return 0;} //file is not found
    // file >>s.b;
    // file >>s.i;
    // file.close();

    // ofstream ofile("out.bin");
    // ofile << s.b;
    // ofile << s.i;
    // ofile.close();

    // FILE *file = fopen("test.bin", "rb");
    //read every part of the structure individually because padding
    // fread(&s.b, sizeof(s.b), 1, file);
    // fread(&s.i, sizeof(s.i), 1, file);
    // cout <<(int)s.b<<endl;
    // cout <<s.i<<endl;
    FILE *file = fopen("test.bin", "wb");
    BYTE y = 5;
    int i = 99;

    fwrite(&y, 1, 1, file);
    fwrite(&i, 4, 1, file);

    fclose(file);

    return 0;
}
