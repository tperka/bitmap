#ifndef BITMAP_H
#define BITMAP_H
#pragma pack(push, 1)

typedef struct tagBITMAPFILEHEADER
{
	unsigned short bfType;	//species the type of file (for bmp it must be 0x4D42
	unsigned int bfSize;  	//represents the size of file
	unsigned short bfReserved1;	//reserved; must be 0
	unsigned short bfReserved2;  //reserved; must be 0
	unsigned int bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct tagBITMAPINFOHEADER
{
	unsigned int biSize;  //specifies the number of bytes required by the struct
	int biWidth;  //specifies width in pixel
	int biHeight;  //species height in pixel
	unsigned short biPlanes; //specifies the number of color planes, must be 1
	unsigned short biBitCount; //specifies the number of bit per pixel
	unsigned int biCompression;//spcifies the type of compression
	unsigned int biSizeImage;  //size of image in bytes
	int biXPelsPerMeter;  //number of pixel per meter in x axis
	int biYPelsPerMeter;  //number of pixel per meter in y axis
	unsigned int biClrUsed;  //number of colors used by th ebitmap
	unsigned int biClrImportant;  //number of colors that are important
}BITMAPINFOHEADER;

#pragma pack(pop)

typedef struct pixel_24bit
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
}pixel_24bit;

void allocCheck(void* p);
pixel_24bit **LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader, BITMAPFILEHEADER* bitmapFileHeader);
void freeBitmap(pixel_24bit** bitmap, BITMAPINFOHEADER fileInfoHeader);
unsigned char* flatBitmap(pixel_24bit** bitmap, BITMAPINFOHEADER *fileInfoHeader);
int checkName(char* fileName);
void cleanBuff();
char *scanFileName();
void saveFile(pixel_24bit** bitmap, BITMAPFILEHEADER* bitmapFileHeader, BITMAPINFOHEADER* bitmapInfoHeader);
void negative(pixel_24bit **bitmap, BITMAPINFOHEADER bitmapInfoHeader);
void greyscale(pixel_24bit **bitmap, BITMAPINFOHEADER bitmapInfoHeader);
int cmpfunc (const void * a, const void * b);
void medianFilter(pixel_24bit** bitmap, int window, BITMAPINFOHEADER bitmapInfoHeader, char filt);

int CharToInt(char c);
int loadInt(char str[], int *ptr);
int scanSize(char size[], BITMAPINFOHEADER bitmapInfo);

#endif 
