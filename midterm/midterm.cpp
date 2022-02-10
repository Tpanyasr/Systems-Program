#include <stdio.h>
#include <iostream>
#include <string.h>
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

    FILE *file1 = fopen("compressed.bin", "rb");
    compressedformat * compressed;
    fread(&(compressed->width),sizeof(int), 1, file1);
    fread(&(compressed->height),sizeof(int), 1, file1);
    fread(&(compressed->rowbyte_quarter),sizeof(int), 4, file1);
    fread(&(compressed->palettecolors),sizeof(int), 1, file1);
    compressed->colors = new col[compressed->palettecolors];

    for(int i = 0; i < compressed->palettecolors; i++)
    {
        fread(&(compressed->colors[i]).r, sizeof(int), 1, file1); 
        fread(&(compressed->colors[i]).g, sizeof(int), 1, file1); 
        fread(&(compressed->colors[i]).b, sizeof(int), 1, file1); 
    }
    //chunk *data = (chunk*)malloc(compressed->rowbyte_quarter[3]*sizeof(chunk));
    chunk *data = new chunk[compressed->rowbyte_quarter[3]];

    int i = 0;
    //while you can still read in chunk (data is not null), read color index and read count
    while (fread(&data[i].color_index, sizeof(BYTE), 1, file1) != 0)
    {
        fread(&data[i].count, sizeof(short), 1, file1);
        i ++;
    }
    fclose(file1);

    tagBITMAPFILEHEADER * bfh;
    bfh->bfType = 19778;
    bfh->bfSize = 4320054;
    bfh->bfReserved1 = 0;
    bfh->bfReserved2 = 0;
    bfh->bfOffBits = 54;

    tagBITMAPINFOHEADER * fih;
    fih->biSize = 40;
    fih->biWidth = 1200;
    fih->biHeight = 1200;
    fih->biPlanes = 1;
    fih->biBitCount = 24;
    fih->biCompression = 0;
    fih->biSizeImage = 4320000;
    fih->biXPelsPerMeter = 3780;
    fih->biYPelsPerMeter = 3780;
    fih->biClrUsed = 0;
    fih->biClrImportant= 0;

    BYTE *idata = new BYTE[fih->biSizeImage];

    

    FILE *outfile = fopen("uncompressed", "wb");
    fwrite(&(bfh->bfType), 2, 1, outfile);
    fwrite(&(bfh->bfSize), 4, 1, outfile);
    fwrite(&(bfh->bfReserved1), sizeof(short), 1, outfile);
    fwrite(&(bfh->bfReserved2), sizeof(short), 1, outfile);
    fwrite(&(bfh->bfOffBits), sizeof(int), 1, outfile);
    fwrite(fih, sizeof(struct tagBITMAPINFOHEADER), 1, outfile);
    fwrite(data, fih->biSizeImage, 1, outfile);

    // int size = compressed->rowbyte_quarter[3];
    // chunk * data =  new chunk[size];
    // int x = 0;
    // while(fread(&(data[x].color_index), sizeof(BYTE), 1, file1) != 0)
    // {
    //     fread(&(data[x].count), sizeof(BYTE), 1, file1);
    //     x++;
    // }
    // printf("%d", x);
    






    
    delete[] data;
    delete[] compressed->colors;

    
    return 0;

}


