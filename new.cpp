#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/sysmacros.h>

// read compressed.bin
typedef unsigned char BYTE;
typedef struct col
{
    int r, g, b;
} col;
typedef struct compressedformat
{
    int width, height;//width and height of the image, 
                    //with one byte for each color, blue, green and red 
    int rowbyte_quarter[4];//for parallel algorithms! That’s the location in bytes 
                            //splitting pts in bytes of the compressed image
                            //which exactly splits the result image after decompression into 4 equal parts!
    int palettecolors; //how many colors does the picture have?
    col *colors; //all participating colors of the image.
} compressedformat;
typedef struct chunk
{
    BYTE color_index; //which of the color of the palette
    short count; //How many pixel of the same color from color_index are continuously appearing 
} chunk;

// write into BMP
typedef unsigned short WORD; 
typedef unsigned int DWORD; 
typedef unsigned int LONG; 
struct tagBITMAPFILEHEADER 
{
    WORD bfType;  //specifies the f type 
    DWORD bfSize;  //specifies the size in bytes of the bitmap f 
    WORD bfReserved1;  //reserved; must be 0 
    WORD bfReserved2;  //reserved; must be 0 
    DWORD bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits 
}; 
struct tagBITMAPINFOHEADER 
{ 
    DWORD biSize;  //specifies the number of bytes required by the struct 
    LONG biWidth;  //specifies width in pixels 
    LONG biHeight;  //species height in pixels 
    WORD biPlanes; //specifies the number of color planes, must be 1 
    WORD biBitCount; //specifies the number of bit per pixel 
    DWORD biCompression;//spcifies the type of compression 
    DWORD biSizeImage;  //size of image in bytes 
    LONG biXPelsPerMeter;  //number of pixels per meter in x axis 
    LONG biYPelsPerMeter;  //number of pixels per meter in y axis 
    DWORD biClrUsed;  //number of colors used by th ebitmap 
    DWORD biClrImportant;  //number of colors that are important 
};

int main()
{
    FILE *f = fopen("compressed.bin", "rb");
    compressedformat input_file;
    fread(&input_file.width, 4, 1, f);
    fread(&input_file.height, 4, 1, f);
    fread(&input_file.rowbyte_quarter, 4, 4, f);
    fread(&input_file.palettecolors, 4, 1, f);
    input_file.colors = (col*)malloc(input_file.palettecolors*sizeof(col));
    for (int i = 0; i < input_file.palettecolors; i ++)
    {
        fread(&input_file.colors[i].r, 4, 1, f);
        fread(&input_file.colors[i].g, 4, 1, f);
        fread(&input_file.colors[i].b, 4, 1, f);
    }
    chunk *color_pair = (chunk*)malloc(input_file.width*input_file.height*sizeof(chunk));
    int j = 0;
    while (fread(&color_pair[j].color_index, 1, 1, f) == 1)
    {
        fread(&color_pair[j].count, 2, 1, f);
        j ++;
    }
    fclose(f);

    struct tagBITMAPFILEHEADER fh;
    struct tagBITMAPINFOHEADER fih;
    // fh set up
    fh.bfType = 19778;
    fh.bfSize = 3*input_file.width*input_file.height + 54;
    fh.bfReserved1 = 0;
    fh.bfReserved2 = 0;
    fh.bfOffBits = 54;
    // fih set up
    fih.biSize = 40;
    fih.biWidth = input_file.width;
    fih.biHeight = input_file.height;
    fih.biPlanes = 1;
    fih.biBitCount = 24;
    fih.biCompression = 0;
    fih.biSizeImage = 3*input_file.width*input_file.height;
    fih.biXPelsPerMeter = 3780;
    fih.biYPelsPerMeter = 3780;
    fih.biClrUsed = 0;
    fih.biClrImportant = 0;

    // decompress in four processes
    col color;
    int n = 0;
    unsigned long biSizeImagef= fih.biSizeImage;
    unsigned char *data = (unsigned char*)mmap(NULL, biSizeImagef, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, 0, 0);
    for (int x = 0; x < fih.biWidth*fih.biHeight; x ++)
    {
        while (color_pair[n].count == 0)
        {
            n ++;
            if (color_pair[n].count != 0)
                break;
        }
        color = input_file.colors[color_pair[n].color_index];
        data[x*3 + 2] = color.r;
        data[x*3 + 1] = color.g;
        data[x*3 + 0] = color.b;
        color_pair[n].count -= 1;
    }
    free(input_file.colors);
    free(color_pair);

    //write to bmp
    f = fopen("resultImage.bmp", "wb+");
    fwrite(&fh.bfType, 2, 1, f);
    fwrite(&fh.bfSize, 4, 1, f);
    fwrite(&fh.bfReserved1, 2, 1, f);
    fwrite(&fh.bfReserved2, 2, 1, f);
    fwrite(&fh.bfOffBits, 4, 1, f);
    fwrite(&fih, sizeof(struct tagBITMAPINFOHEADER), 1, f);
    fwrite(data, fih.biSizeImage, 1, f);
    munmap(&data, biSizeImagef);
    fclose(f);
    return 0;
}