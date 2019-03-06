#include <stdio.h>
#include "filters.h"
#include "bitmap.h"

pixel_24bit median(pixel_24bit **matrix, int sizev, int sizeh)
{
	unsigned char medianB, medianG, medianR;
	unsigned char *B = malloc(sizev*sizeh), *G = malloc(sizev*sizeh), *R = malloc(sizev*sizeh);
	allocCheck(B);
	allocCheck(G);
	allocCheck(R);
	for(int i = 0; i < sizev; i++)
	{
		for(int j = 0; j < sizeh; j++)
		{
			B[j + i*sizeh] = matrix[i][j].b;
			G[j + i*sizeh] = matrix[i][j].g;
			R[j + i*sizeh] = matrix[i][j].r;
		}
	}
	qsort(B, sizev*sizeh, sizeof(unsigned char), cmpfunc);
	qsort(G, sizev*sizeh, sizeof(unsigned char), cmpfunc);
	qsort(R, sizev*sizeh, sizeof(unsigned char), cmpfunc);
	if((sizev*sizeh) % 2 == 0)
	{
		medianB = (B[(sizev*sizeh)/2]+B[(sizev*sizeh)/2 - 1])/2;
		medianG = (G[(sizev*sizeh)/2]+G[(sizev*sizeh)/2 - 1])/2;
		medianR = (R[(sizev*sizeh)/2]+R[(sizev*sizeh)/2 - 1])/2;
	}
	else
	{
		medianB = B[(sizev*sizeh)/2];
		medianG = G[(sizev*sizeh)/2];
		medianR = R[(sizev*sizeh)/2];
	}
	free(B);
	free(G);
	free(R);
	pixel_24bit result = {medianB, medianG, medianR};
	return result;
}


pixel_24bit average(pixel_24bit **matrix, int sizev, int sizeh)
{
	unsigned char averageB, averageG, averageR;
	unsigned char *B = malloc(sizev*sizeh), *G = malloc(sizev*sizeh), *R = malloc(sizev*sizeh);
	allocCheck(B);
	allocCheck(G);
	allocCheck(R);
	for(int i = 0; i < sizev; i++)
	{
		for(int j = 0; j < sizeh; j++)
		{
			B[j + i*sizeh] = matrix[i][j].b;
			G[j + i*sizeh] = matrix[i][j].g;
			R[j + i*sizeh] = matrix[i][j].r;
		}
	}
	int helpB = 0, helpG=0, helpR=0;
	for(int i = 0; i < sizev*sizeh; i++)
		{
			helpB += B[i];
			helpG += G[i];
			helpR += R[i];
		}
	averageB = helpB/(sizev*sizeh);
	averageG = helpG/(sizev*sizeh);
	averageR = helpR/(sizev*sizeh);
	free(B);
	free(G);
	free(R);
	pixel_24bit result = {averageB, averageG, averageR};
	return result;
}



pixel_24bit minimal(pixel_24bit **matrix, int sizev, int sizeh)
{
	unsigned char minB, minG, minR;
	unsigned char *B = malloc(sizev*sizeh), *G = malloc(sizev*sizeh), *R = malloc(sizev*sizeh);
	allocCheck(B);
	allocCheck(G);
	allocCheck(R);
	for(int i = 0; i < sizev; i++)
	{
		for(int j = 0; j < sizeh; j++)
		{
			B[j + i*sizeh] = matrix[i][j].b;
			G[j + i*sizeh] = matrix[i][j].g;
			R[j + i*sizeh] = matrix[i][j].r;
		}
	}
	qsort(B, sizev*sizeh, sizeof(unsigned char), cmpfunc);
	qsort(G, sizev*sizeh, sizeof(unsigned char), cmpfunc);
	qsort(R, sizev*sizeh, sizeof(unsigned char), cmpfunc);
	minB = B[0];
	minG = G[0];
	minR = R[0];
	free(B);
	free(G);
	free(R);
	pixel_24bit result = {minB, minG, minR};
	return result;
}



pixel_24bit max(pixel_24bit **matrix, int sizev, int sizeh)
{
	unsigned char maxB, maxG, maxR;
	unsigned char *B = malloc(sizev*sizeh), *G = malloc(sizev*sizeh), *R = malloc(sizev*sizeh);
	allocCheck(B);
	allocCheck(G);
	allocCheck(R);
	for(int i = 0; i < sizev; i++)
	{
		for(int j = 0; j < sizeh; j++)
		{
			B[j + i*sizeh] = matrix[i][j].b;
			G[j + i*sizeh] = matrix[i][j].g;
			R[j + i*sizeh] = matrix[i][j].r;
		}
	}
	qsort(B, sizev*sizeh, sizeof(unsigned char), cmpfunc);
	qsort(G, sizev*sizeh, sizeof(unsigned char), cmpfunc);
	qsort(R, sizev*sizeh, sizeof(unsigned char), cmpfunc);
	maxB = B[sizev*sizeh - 1];
	maxG = G[sizev*sizeh - 1];
	maxR = R[sizev*sizeh - 1];
	free(B);
	free(G);
	free(R);
	pixel_24bit result = {maxB, maxG, maxR};
	return result;
}