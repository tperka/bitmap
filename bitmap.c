#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "bitmap.h"
int err = 0;

void allocCheck(void* p)
{
	if(p == NULL)
	{
		fputs("Brak pamięci.\n", stderr);
		exit(EXIT_FAILURE);
	}
}



pixel_24bit **LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader, BITMAPFILEHEADER* bitmapFileHeader)
{
	FILE *filePtr;
	filePtr = fopen(filename,"rb");
	if (filePtr == NULL)
        {
        	err = 1;
        	return NULL;
	}

	fread(bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);


	if (bitmapFileHeader->bfType !=0x4D42)
	{
		err = 2;
	        fclose(filePtr);
	        return NULL;
	}


	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr);
	fseek(filePtr, bitmapFileHeader->bfOffBits, SEEK_SET);
	if(bitmapInfoHeader->biBitCount != 24 || bitmapInfoHeader->biPlanes != 1 || bitmapFileHeader->bfReserved1 !=0 || bitmapFileHeader->bfReserved2 != 0)
	{
		err = 2;
		fclose(filePtr);
		return NULL;
	}
  
	pixel_24bit **bitmap = malloc(bitmapInfoHeader->biSizeImage);

	for(int i = 0; i < bitmapInfoHeader->biHeight; i++)
    	{
	    	unsigned char* bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biWidth * 3);
	    	pixel_24bit* row = malloc(bitmapInfoHeader->biWidth*sizeof(pixel_24bit));
	    	allocCheck(bitmapImage);
	    	allocCheck(row);
	    	fread(bitmapImage,bitmapInfoHeader->biWidth*3, 1, filePtr);
	    	for(int j = 0; j < bitmapInfoHeader->biWidth; j++)
	    	{
	    		row[j].b = bitmapImage[3*j];
	    		row[j].g = bitmapImage[3*j+1];
	    		row[j].r = bitmapImage[3*j+2];
	    		
	    	}
		bitmap[i] = row;
		free(bitmapImage);
	}
	
	if (bitmap == NULL)
	{
	        err = 3;
	        fclose(filePtr);
	        return NULL;
	}

	fclose(filePtr);
	return bitmap;
}

void freeBitmap(pixel_24bit** bitmap, BITMAPINFOHEADER fileInfoHeader)
{
	for(int i = 0; i < fileInfoHeader.biHeight; i++)
		free(bitmap[i]);
		
	free(bitmap);
}
unsigned char* flatBitmap(pixel_24bit** bitmap, BITMAPINFOHEADER *fileInfoHeader)
{
	unsigned char* flatBitmap = malloc(fileInfoHeader->biSizeImage);
	for(int i = 0; i < fileInfoHeader->biHeight; i++)
	{
		for(int j = 0; j < fileInfoHeader->biWidth; j++)
		{
			flatBitmap[3*j + i * fileInfoHeader->biWidth * 3] = bitmap[i][j].b;
			flatBitmap[3*j + 1 + i * fileInfoHeader->biWidth * 3] = bitmap[i][j].g;
			flatBitmap[3*j + 2 + i * fileInfoHeader->biWidth * 3] = bitmap[i][j].r;
		}
	}
	return flatBitmap;
}
int checkName(char* fileName)
{
	if(strlen(fileName) > 30)
		return 0;
		
	for(int i = 0; i < strlen(fileName); i++)
	{
		if(fileName[i] == '/')
			return 0;
	}
	return 1;
}

void cleanBuff()
{
	char c = getchar();
	while( c != '\n')
		c = getchar();
}

char *scanFileName()
{
	char *fileName = malloc(32);
	printf("Podaj nazwę pliku bez rozszerzenia.\n");
	while(scanf("%31[^\n]", fileName) == 0 || checkName(fileName) == 0)
	{
		cleanBuff();
		printf("Niepoprawna wartość. Wprowadź poprawną nazwę pliku, conajwyżej 30 znaków.\n");
	}
	cleanBuff();
	return strcat(fileName, ".bmp");
}

void saveFile(pixel_24bit** bitmap, BITMAPFILEHEADER* bitmapFileHeader, BITMAPINFOHEADER* bitmapInfoHeader)
{
	unsigned char* bitmapData = flatBitmap(bitmap, bitmapInfoHeader);
	char *fileName = scanFileName();
	FILE* filePtr = fopen(fileName,"wb");
    	if (filePtr == NULL)
       	{
       		fputs("Nie udało się nadpisać pliku.\n", stderr);
		exit(EXIT_FAILURE);
       	}	
       	fwrite(bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
       	fwrite(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
       	fseek(filePtr, bitmapFileHeader->bfOffBits, SEEK_SET);
       	fwrite(bitmapData, sizeof(unsigned char), bitmapInfoHeader->biSizeImage, filePtr);
       	free(bitmapData);
       	free(fileName);
       	fclose(filePtr);
}


void negative(pixel_24bit **bitmap, BITMAPINFOHEADER bitmapInfoHeader)
{
	for(int i = 0; i < bitmapInfoHeader.biHeight; i++)
	{
		for(int j = 0; j < bitmapInfoHeader.biWidth; j++)
		{
			bitmap[i][j].b *= -1;
			bitmap[i][j].g *= -1;
			bitmap[i][j].r *= -1;
		}
	}
}

void greyscale(pixel_24bit **bitmap, BITMAPINFOHEADER bitmapInfoHeader)
{
	for(int i = 0; i < bitmapInfoHeader.biHeight; i++)
	{
		for(int j = 0; j < bitmapInfoHeader.biWidth; j++)
		{
			bitmap[i][j].b = bitmap[i][j].r = bitmap[i][j].g = 0.2989*bitmap[i][j].r + 0.5870*bitmap[i][j].g + 0.1140*bitmap[i][j].b;
		}
	
	}
}

int cmpfunc (const void * a, const void * b) {
   return ( *(unsigned char*)a - *(unsigned char*)b );
}



void filter(pixel_24bit** bitmap, int window, BITMAPINFOHEADER bitmapInfoHeader, char filt)
{
	int vertCount = bitmapInfoHeader.biHeight / window;
	int horCount = bitmapInfoHeader.biWidth / window;
	for(int i = 0; i < vertCount; i++)
	{
		for(int j = 0; j < horCount; j++)
		{
			pixel_24bit** matrix = malloc(window*window*sizeof(pixel_24bit));
			allocCheck(matrix);
			for(int k = 0; k < window; k++)
			{
				pixel_24bit* row = malloc(window*sizeof(pixel_24bit));
				for(int l = 0; l < window; l++)
				{
					row[l] = bitmap[k + window * i][l + window * j];
				}
				matrix[k] = row;
			}
			for(int m = 0; m < window; m++)
			{
				for(int n = 0; n < window; n++)
				{
					if(filt == 'a')
						bitmap[m + window * i][n + window * j] = median(matrix, window, window);
					else if(filt == 'b')
						bitmap[m + window * i][n + window * j] = average(matrix, window, window);
					else if(filt== 'c')
						bitmap[m + window * i][n + window * j] = max(matrix, window, window);
					else if(filt== 'd')
						bitmap[m + window * i][n + window * j] = minimal(matrix, window, window);
				}
			}
			for(int o = 0; o < window; o++)
				free(matrix[o]);
				
			free(matrix);
		}
			
	}
	int addWidth = bitmapInfoHeader.biWidth % window;
	for(int i = 0; i < vertCount; i++)
	{
	
		pixel_24bit** matrix1 = malloc(window*addWidth*sizeof(pixel_24bit));
		allocCheck(matrix1);

		for(int k = 0; k < window; k++)
		{
			pixel_24bit* row1 = malloc(addWidth*sizeof(pixel_24bit));
			for(int l = 0; l < addWidth; l++)
			{
				row1[l] = bitmap[k + window * i][l + window*horCount];
			}
			matrix1[k] = row1;
		}
		for(int m = 0; m < window; m++)
		{
			for(int n = 0; n < addWidth; n++)
			{
				if(filt == 'a')
					bitmap[m + window * i][n + window * j] = median(matrix, window, window);
				else if(filt == 'b')
					bitmap[m + window * i][n + window * j] = average(matrix, window, window);
				else if(filt== 'c')
					bitmap[m + window * i][n + window * j] = max(matrix, window, window);
				else if(filt== 'd')
					bitmap[m + window * i][n + window * j] = minimal(matrix, window, window);
			}
		}
		for(int o = 0; o < window; o++)	
			free(matrix1[o]);
				
		free(matrix1);
			
	}
	int addHeight = bitmapInfoHeader.biHeight % window;
	for(int j = 0; j < horCount; j++)
	{
		pixel_24bit** matrix2 = malloc(addHeight*window*sizeof(pixel_24bit));
		allocCheck(matrix2);
		for(int k = 0; k < addHeight; k++)
		{
			pixel_24bit* row2 = malloc(window*sizeof(pixel_24bit));
			for(int l = 0; l < window; l++)
			{
				row2[l] = bitmap[k + window * vertCount][l + window * j];
			}
			matrix2[k] = row2;
		}
		for(int m = 0; m <addHeight; m++)
		{
			for(int n = 0; n < window; n++)
			{
				if(filt == 'a')
					bitmap[m + window * i][n + window * j] = median(matrix, window, window);
				else if(filt == 'b')
					bitmap[m + window * i][n + window * j] = average(matrix, window, window);
				else if(filt== 'c')
					bitmap[m + window * i][n + window * j] = max(matrix, window, window);
				else if(filt== 'd')
					bitmap[m + window * i][n + window * j] = minimal(matrix, window, window);
			}
		}
		for(int o = 0; o < addHeight; o++)
			free(matrix2[o]);
			
		free(matrix2);
	}
	
	pixel_24bit** matrix3 = malloc(addHeight*addWidth*sizeof(pixel_24bit));
	allocCheck(matrix3);
	for(int k = 0; k < addHeight; k++)
	{
		pixel_24bit *row3 = malloc(addWidth*sizeof(pixel_24bit));
		for(int l = 0; l < addWidth; l++)
		{
			row3[l] = bitmap[k + window * vertCount][l + window * horCount];
		}
		matrix3[k] = row3;
	}	
	for(int m = 0; m <addHeight; m++)
	{
		for(int n = 0; n < addWidth; n++)
		{
			if(filt == 'a')
				bitmap[m + window * i][n + window * j] = median(matrix, window, window);
			else if(filt == 'b')
				bitmap[m + window * i][n + window * j] = average(matrix, window, window);
			else if(filt== 'c')
				bitmap[m + window * i][n + window * j] = max(matrix, window, window);
			else if(filt== 'd')
				bitmap[m + window * i][n + window * j] = minimal(matrix, window, window);
		}
	}
	for(int o = 0; o < addHeight; o++)
		free(matrix3[o]);
		
	free(matrix3);
}	

int CharToInt(char c)
{
	if(c == '0')	return 0;
	if(c == '1')	return 1;
	if(c == '2')	return 2;
	if(c == '3')	return 3;
	if(c == '4')	return 4;
	if(c == '5')	return 5;
	if(c == '6')	return 6;
	if(c == '7')	return 7;
	if(c == '8')	return 8;
	if(c == '9')	return 9;
	return 10;
}

int loadInt(char str[], int *ptr)
{
	if(strlen(str) >= 1 && str[0] == '0')
		return 0;
	if(strlen(str) == 1 && str[0] == '1')
		return 0;
	
	for(int i = 0; i < strlen(str); i++)
	{
		int a = CharToInt(str[i]);
		if(a == 10)
			return 0;
		else
			*ptr += a*pow(10, strlen(str)-i-1);
	}
	return 1;
}

int scanSize(char size[], BITMAPINFOHEADER bitmapInfo)
{
	int res = 0, min;
	int *ptr = &res;
	if(bitmapInfo.biWidth < bitmapInfo.biHeight)
		min = bitmapInfo.biWidth;
	else
		min = bitmapInfo.biHeight;
		
	printf("Podaj rozmiar okna:\n");
	while(scanf("%5[^\n]", size) == 0 || strlen(size) > 4 || loadInt(size, ptr) == 0 || res > min)
	{
		res = 0;
		printf("Niepoprawne polecenie. Użyj liczby z zakresu 2-%d. Spróbuj ponownie.\n", min);
		cleanBuff();
	}
	cleanBuff();
	return res;
}


