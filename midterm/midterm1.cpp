#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <vector>
#include <sys/wait.h>
using namespace std;
#define BLUE 0
#define GREEN 1
#define RED 2

typedef unsigned char BYTE;


// Midterm Assignment
// CPE 357
// Systems Programming
// MIDTERM
// Exercise:
// Write a decompression program to decompress an compressed image file into a bitmap. Speed
// it up with parallel algorithm.
// The compression format:
// You will find a file named compressed.bin. This is a new image file format, invented by your
// instructor for this midterm. It could reduce a 4 MB image to nearly 100 kB!
// The format is as follows, beginning with a header:



typedef struct col
{
    int r,g,b; //red, green and blue, in that order. The values will not exceed 255!
} col;


typedef struct compressedformat
{
    int width, height; //width and height of the image, with one byte for each color, blue, green and red
    int rowbyte_quarter[4];//for parallel algorithms! That’s the location in bytes
                            // which exactly splits the result image after decompression into 4 equal
                            // parts!
    int palettecolors; //how many colors does the picture have?
    col * colors;
} compressedformat;


  

// After the header “compressedformat” follows the data part.
// The data part consists of chunks:
typedef struct chunk
{
    BYTE color_index; //which of the color of the palette
    short count;//How many pixel of the same color from color_index are continuously appearing
} chunk;

typedef struct tagBITMAPFILEHEADER
{
   short bfType;  //specifies the file type
    int bfSize;  //specifies the size in bytes of the bitmap file
   short bfReserved1;  //reserved; must be 0
   short bfReserved2;  //reserved; must be 0
    int bfOffBits;  //specifies the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{ 
    int biSize;  //specifies the number of bytes required by the struct
    int biWidth;  //specifies width in pixels
    int biHeight;  //specifies height in pixels
   short biPlanes;  //specifies the number of color planes, must be 1
   short biBitCount;  //specifies the number of bits per pixel
    int biCompression;  //specifies the type of compression
    int biSizeImage;  //size of image in bytes
    int biXPelsPerMeter;  //number of pixels per meter in x axis
    int biYPelsPerMeter;  //number of pixels per meter in y axis
    int biClrUsed;  //number of colors used by the bitmap
    int biClrImportant;  //number of colors that are important
}BITMAPINFOHEADER;



int main(int argc, char * argv[])
{
    int z = (1440000*2)/4;
    int m = (1440000) * (2/4);

    


    FILE *file1 = fopen("compressed.bin", "rb");
    compressedformat compressed;
    fread(&(compressed.width), 4, 1, file1);
    fread(&(compressed.height),4, 1, file1);
    fread(&(compressed.rowbyte_quarter),4, 4, file1);
    fread(&(compressed.palettecolors),4, 1, file1);
    compressed.colors = new col[compressed.palettecolors];

    for(int i = 0; i < compressed.palettecolors; i++)
    {
        fread(&(compressed.colors[i]).r, 4, 1, file1); 
        fread(&(compressed.colors[i]).g, 4, 1, file1); 
        fread(&(compressed.colors[i]).b, 4, 1, file1); 
    }
    //chunk *data = (chunk*)malloc(compressed->rowbyte_quarter[3]*sizeof(chunk));
    chunk * data = new chunk[compressed.rowbyte_quarter[3]];

    int i = 0;
    //if you can still read in chunk (data is not null), read color index and read count
    while(1)
    {
        if(fread(&data[i].color_index, 1, 1, file1))
        {
            fread(&data[i].count, 2, 1, file1);
            i++;
        }
        else
            break;
    }
    //read in the bmp headers
    tagBITMAPFILEHEADER bfh;
    bfh.bfType = 19778;
    bfh.bfSize = 4320054;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfOffBits = 54;
    tagBITMAPINFOHEADER fih;
    fih.biSize = 40;
    fih.biWidth = 1200;
    fih.biHeight = 1200;
    fih.biPlanes = 1;
    fih.biBitCount = 24;
    fih.biCompression = 0;
    fih.biSizeImage = 4320000;
    fih.biXPelsPerMeter = 3780;
    fih.biYPelsPerMeter = 3780;
    fih.biClrUsed = 0;
    fih.biClrImportant= 0;

    BYTE *idata = (BYTE *) mmap(NULL, fih.biSizeImage, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    int x = 0;
    int idx = 0;

    //fork
    clock_t a = clock();

    for(int x = 0; x < (fih.biSizeImage/3); x++)
    {
        while(data[idx].count==0)
        {
            idx++;
        }
        idata[(x*3) + BLUE] = compressed.colors[data[idx].color_index].b;
        idata[(x*3) + GREEN] = compressed.colors[data[idx].color_index].g;
        idata[(x*3) + RED] = compressed.colors[data[idx].color_index].r;
        data[idx].count--; 
        if(x == fih.biSizeImage/3 -1)
        {
            int g = 0;
        }
    }
    a = clock();
    std::cout << "Time for Parallel [With fork]: "<< a <<endl;

    file1 = fopen("other.bmp", "wb");
    fwrite(&bfh.bfType, 2, 1, file1);
    fwrite(&bfh.bfSize, 4, 1, file1);
    fwrite(&bfh.bfReserved1, 2, 1, file1);
    fwrite(&bfh.bfReserved2, 2, 1, file1);
    fwrite(&bfh.bfOffBits, 4, 1, file1);
    fwrite(&fih, sizeof(struct tagBITMAPINFOHEADER), 1, file1);
    fwrite(idata, fih.biSizeImage, 1, file1);
    fclose(file1);
    //don't forget to delete stuff    
    return 0;

}


