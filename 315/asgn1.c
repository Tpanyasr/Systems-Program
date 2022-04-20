#include <stdio.h>
#include <string.h>





void matMult (int **first, int **second, int **multiply, int height, int width)
{
    int sum = 0;
    for (int c = 0; c < width; c++) {

        for (int d = 0; d < height; d++) {

            for (int k = 0; k < height; k++) {

            sum = sum + first[c][k]*second[k][d];

            }

            multiply[c][d] = sum;

            sum = 0;

        }

        }
}
void display(int result[256][256], int row, int column) {

   printf("\nOutput Matrix:\n");
   for (int i = 0; i < row; ++i) {
      for (int j = 0; j < column; ++j) {
         printf("%d  ", result[i][j]);
         if (j == column - 1)
            printf("\n");
      }
   }
}


int main()
{
    int first[256][256];
    int second[256][256];
    int result[256][256];
    for(int x = 0; x <256; x++)
    {
        for(int y = 0; y<256; y++)
        {
            first[x][y] = rand() %1 + 0;
            second[x][y] = rand() %1 + 0;

        }
    }
    matMult(first, second, result, 256, 256);
    display(result, 256, 256);
    return 0;
}
