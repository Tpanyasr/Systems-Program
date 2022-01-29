#include <stdio.h>
#include <iostream>
#include <string.h>
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
BYTE *blend(BYTE* idata1, BYTE *idata2, BYTE* idataFinal, tagBITMAPINFOHEADER *fih1,tagBITMAPINFOHEADER *fih2, float ratio, char *infile1, char *infile2)
{



    //initialize the merged image's values
    if(fih1->biWidth ==  fih2->biWidth && fih2->biHeight == fih1->biHeight)
    {
        
       
        for(int y = 0; y < fih1->biHeight; y++)
        {
            int realwidth =  y *fih1->biWidth*3;
            if(realwidth %4 != 0)
                realwidth = realwidth + 1 - (realwidth %4);
            
            for(int x = 0; x < fih1->biWidth; x++)
            {
                
                
                //ratio blue
                
                BYTE blue1 = getColor(idata1, fih1->biWidth, x, y, BLUE);
                BYTE blue2 = getColor(idata2, fih2->biWidth, x, y, BLUE);
                BYTE blue_result = (blue1 *ratio) + (blue2 * (1- ratio));
                idataFinal[(x*3) + (realwidth) + BLUE] = blue_result;
               
                BYTE green1 = getColor(idata1, fih1->biWidth, x, y, GREEN);
                BYTE green2 = getColor(idata2, fih2->biWidth, x, y, GREEN);
                BYTE green_result =(green1 *ratio) + (green2 * (1- ratio));
                idataFinal[(x*3) + (realwidth) + GREEN] = green_result;

                BYTE red1 = getColor(idata1, fih1->biWidth, x, y, RED);
                BYTE red2 = getColor(idata2, fih2->biWidth, x, y, RED);
                BYTE red_result =(red1 *ratio) + (red2 * (1- ratio));
                idataFinal[(x*3) + (realwidth) + RED] = red_result;

            }
        }
    
    
    }
    else
    {
        printf("\n\nSorry! Bilinear Interpolation has not been added. Choose images of the same size!\n\n");

        exit(EXIT_FAILURE);
    }


    return idataFinal;

    

}

void write(char *output, tagBITMAPFILEHEADER *bfh1, tagBITMAPINFOHEADER *fih1, BYTE*data)
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
    //     perror("ERROR Usage:[programname][imagefile1][imagefile2][ratio][outputfile]");
    //     exit(EXIT_FAILURE);
    // }


    FILE *file1 = fopen(argv[1],"rb");
    FILE *file2 = fopen(argv[2],"rb");
    if(!file1)
    {
    printf("\n\nUsage: [programname][imagefile1][imagefile2][ratio][outputfile] \n\n");
        exit(EXIT_FAILURE);
    }
    if(!file2)
    {
    printf("\n\nUsage: [programname][imagefile1][imagefile2][ratio][outputfile] \n\n");
        exit(EXIT_FAILURE);
    }
    fclose(file1);
    fclose(file2);
    float ratio = atof(argv[3]);
    if(ratio >=1 || ratio <= 0)
    {
    printf("\n\nUsage: [programname][imagefile1][imagefile2][ratio][outputfile] \n\n");
        exit(EXIT_FAILURE);
    } 

    tagBITMAPFILEHEADER bfh1, bfh2;
    tagBITMAPINFOHEADER fih1, fih2;
    BYTE *idata1, *idata2;
    idata1 = readFile(argv[1], &bfh1, &fih1);
    idata2 = readFile(argv[2], &bfh2, &fih2);
    BYTE *idataFinal = new BYTE[fih1.biSizeImage];
    idataFinal = blend(idata1, idata2, idataFinal, &fih1, &fih2, ratio, argv[1], argv[2]);
    write(argv[4], &bfh1, &fih1, idataFinal);
    delete(idata1);
    delete(idata2);

    
    return 0;

}

