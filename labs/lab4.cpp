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




BYTE getColor(BYTE *data, int width, int x, int y, int color)
{
 
    unsigned char colreturn;
    int realwidth = y * width * 3;
    if (realwidth % 4 != 0){
        realwidth = realwidth + 4 - (realwidth % 4);
    }

    colreturn = data[(x * 3) + realwidth + color];

    return colreturn;
}

BYTE * readFile(char * file, BITMAPFILEHEADER *bfh1, BITMAPINFOHEADER *fih1)
{
     //OPEN FILE 
    FILE *file1 = fopen(file, "rb");
    //READ FILE1
    fread(&(bfh1->bfType), 2, 1, file1);
    fread(&(bfh1->bfSize), 4, 1, file1);
    fread(&(bfh1->bfReserved1), sizeof(short), 1, file1);
    fread(&(bfh1->bfReserved2), sizeof(short), 1, file1);
    fread(&(bfh1->bfOffBits), sizeof(int), 1, file1);
    fread(fih1, sizeof(struct tagBITMAPINFOHEADER), 1, file1);
    BYTE *idata1 = new BYTE[fih1->biSizeImage];
    fread(idata1, fih1->biSizeImage, 1, file1);
    fclose(file1);
    return idata1;

}

BYTE *change_withoutFork(BYTE* idata1,tagBITMAPINFOHEADER *fih1, float brightness)
{

       clock_t a = clock();
        for(int y = 0; y < fih1->biHeight; y++)
        {
            int realwidth =  y *fih1->biWidth*3;
            if(realwidth %4 != 0)
                realwidth = realwidth + 1 - (realwidth %4);
            
            for(int x = 0; x < fih1->biWidth; x++)
            {
                
                
                //ratio blue
                
                int blue1 = (int)(getColor(idata1, fih1->biWidth, x, y, BLUE));
                int blue_result = blue1 + brightness;
                if(blue_result > 255)
                    blue_result = 255;
                idata1[(x*3) + (realwidth) + BLUE] = blue_result;
               
                int green1 = (int)(getColor(idata1, fih1->biWidth, x, y, GREEN));
                int green_result = green1 + brightness;
                if(green_result > 255)
                    green_result = 255;
                idata1[(x*3) + (realwidth) + GREEN] = green_result;

                int red1 = (int)(getColor(idata1, fih1->biWidth, x, y, RED));
                int red_result = red1 + brightness;
                if(red_result > 255)
                    red_result = 255;
                idata1[(x*3) + (realwidth) + RED] = red_result;

            }
        }
    
    a = clock();
    std::cout<< "Time for Parallel [Without fork]: "<< a<<endl;

    return idata1;

    

}

BYTE *change_withFork(BYTE* idata1,tagBITMAPINFOHEADER *fih1, float brightness)
{
    int half;

    if(fih1->biHeight %2 == 0)
        half = fih1->biHeight/2;
    else
        half = fih1->biHeight/2 + 1;

    BYTE *shared = (BYTE *) mmap(NULL, fih1->biSizeImage, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    clock_t a = clock();
    if(fork() == 0){        //child process

        for(int y = half; y < fih1->biHeight; y++)
        {
            int realwidth =  y *fih1->biWidth*3;
            if(realwidth %4 != 0)
                realwidth = realwidth + 1 - (realwidth %4);
            
            for(int x = 0; x < fih1->biWidth; x++)
            {
                
                
                //ratio blue
                
                int blue1 = (int)(getColor(idata1, fih1->biWidth, x, y, BLUE));
                int blue_result = blue1 + brightness;
                if(blue_result > 255)
                    blue_result = 255;
                shared[(x*3) + (realwidth) + BLUE] = blue_result;
               
                int green1 = (int)(getColor(idata1, fih1->biWidth, x, y, GREEN));
                int green_result = green1 + brightness;
                if(green_result > 255)
                    green_result = 255;
                shared[(x*3) + (realwidth) + GREEN] = green_result;

                int red1 = (int)(getColor(idata1, fih1->biWidth, x, y, RED));
                int red_result = red1 + brightness;
                if(red_result > 255)
                    red_result = 255;
                shared[(x*3) + (realwidth) + RED] = red_result;

            }
        }
        
            return 0;
    }
    else{

     //parent process

        for(int y = 0; y < half; y++)
        {
            int realwidth =  y *fih1->biWidth*3;
            if(realwidth %4 != 0)
                realwidth = realwidth + 1 - (realwidth %4);
            
            for(int x = 0; x < fih1->biWidth; x++)
            {
                
                
                //ratio blue
                
                int blue1 = (int)(getColor(idata1, fih1->biWidth, x, y, BLUE));
                int blue_result = blue1 + brightness;
                if(blue_result > 255)
                    blue_result = 255;
                shared[(x*3) + (realwidth) + BLUE] = blue_result;
               
                int green1 = (int)(getColor(idata1, fih1->biWidth, x, y, GREEN));
                int green_result = green1 + brightness;
                if(green_result > 255)
                    green_result = 255;
                shared[(x*3) + (realwidth) + GREEN] = green_result;

                int red1 = (int)(getColor(idata1, fih1->biWidth, x, y, RED));
                int red_result = red1 + brightness;
                if(red_result > 255)
                    red_result = 255;
                shared[(x*3) + (realwidth) + RED] = red_result;

            }
        }
       
    }
    wait(0);
    a = clock();
    std::cout << "Time for Parallel [With fork]: "<< a <<endl;
    return shared;

}

void writebmp(char *output, tagBITMAPFILEHEADER *bfh1, tagBITMAPINFOHEADER *fih1, BYTE*data)
{
    FILE *outfile = fopen(output, "wb+");
    fwrite(&(bfh1->bfType), 2, 1, outfile);
    fwrite(&(bfh1->bfSize), 4, 1, outfile);
    fwrite(&(bfh1->bfReserved1), sizeof(short), 1, outfile);
    fwrite(&(bfh1->bfReserved2), sizeof(short), 1, outfile);
    fwrite(&(bfh1->bfOffBits), sizeof(int), 1, outfile);
    fwrite(fih1, sizeof(struct tagBITMAPINFOHEADER), 1, outfile);
    fwrite(data, fih1->biSizeImage, 1, outfile);
   
    fclose(outfile);

}

int main(int argc, char * argv[])
{

    // if(argc != 5){
    //     printf("\n\nUsage: [programname][imagefile1][brightness][parallel][outputfile] \n\n");
    //     exit(EXIT_FAILURE);
    // }


    FILE *file1 = fopen(argv[1],"rb");
    if(!file1)
    {
    printf("\n\nUsage: [programname][imagefile1][brightness][parallel][outputfile] \n\n");
        exit(EXIT_FAILURE);
    }
    fclose(file1);
    float brightness = atof(argv[2]);
    if(brightness >1 || brightness < 0)
    {
    printf("\n\nUsage: [programname][imagefile1][brightness][parallel][outputfile] \n\n");
        exit(EXIT_FAILURE);
    } 
    int parallel = atoi(argv[3]);
    if(parallel == 1 || parallel == 0)
    {
       
    brightness = brightness * 255;
    

    tagBITMAPFILEHEADER bfh1;
    tagBITMAPINFOHEADER fih1;
    BYTE *idata1;

    idata1 = readFile(argv[1], &bfh1, &fih1);
    if(parallel == 0)
    {
        idata1 = change_withoutFork(idata1, &fih1,brightness);
        writebmp(argv[4], &bfh1, &fih1, idata1);
        delete(idata1);
    }
    else
    {
        if(parallel == 1)
            {
                idata1 = change_withFork(idata1, &fih1, brightness);
                writebmp(argv[4], &bfh1, &fih1, idata1);
                munmap(idata1, fih1.biSizeImage);
            }
        }
    }
    else{
         printf("\n\nUsage: [programname][imagefile1][brightness][parallel][outputfile] \n\n");
        exit(EXIT_FAILURE);
    }

    
  


    
    return 0;
}

