#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "bitmap.h"
#include "filters.h"

extern int err;

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("Błędne dane wejściowe!\n");
		return 1;
	}
	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER bitmapFileHeader;
    	pixel **bitmapData;
    	char menu[3], window[6];
    	int windowSize = 0;
    	bitmapData = LoadBitmapFile(argv[1], &bitmapInfoHeader, &bitmapFileHeader);
	if(err == 1)
	{
		printf("Nie udało się otworzyć pliku. Sprawdź, czy na pewno podałeś właściwą nazwę.\n");
		return 1;
	}
	else if(err == 2)
	{
		printf("Nieobsługiwany format pliku. Proszę użyć 24 bitowego pliku bmp.\n");
		return 1;
	}
	else if(err == 3)
	{
		printf("Nie udało się wczytać bitmapy. Sprawdź, czy plik nie jest uszkodzony.\n");
		return 1;
	}
	printf("Pomyślnie wczytano plik.\n");
    	while(1)
    	{
    		printf("Co chciałbyś zrobić?\n1. Odszumić filtrem medianowym\n2. Odszumić filtrem średnim\n3. Odszumić filtrem minimalnym\n4. Odszumić filtrem maksymalnym\n5. Wygenerować negatyw\n6. Wygenerować obraz w skali szarości\n7. Zapisać aktualny obraz do pliku\n8. Wyjść\n");
		while(scanf("%2[^\n]", menu) == 0 || strlen(menu) > 1 || menu[0] >'8' || menu[0] < '1')
		{
			printf("Niepoprawne polecenie. Użyj liczby z zakresu 1-8. Spróbuj ponownie.\n");
			cleanBuff();
		}
		cleanBuff();
    		
    		if(menu[0] == '1')
    		{
    			windowSize = scanSize(window, bitmapInfoHeader);
    			medianFilter(bitmapData, windowSize, bitmapInfoHeader);
    			
    		}
    		else if(menu[0] == '2')
    		{
    			windowSize = scanSize(window, bitmapInfoHeader);
    			averageFilter(bitmapData, windowSize, bitmapInfoHeader);
    		}
    		else if(menu[0] == '3')
    		{
    			windowSize = scanSize(window, bitmapInfoHeader);
    			minimalFilter(bitmapData, windowSize, bitmapInfoHeader);
    		}
    		else if(menu[0] == '4')
    		{
    			windowSize = scanSize(window, bitmapInfoHeader);
    			maxFilter(bitmapData, windowSize, bitmapInfoHeader);
    		}
    		else if(menu[0] == '5')
    		{
    			negative(bitmapData, bitmapInfoHeader);
    		}
    		else if(menu[0] == '6')
    		{
    			greyscale(bitmapData, bitmapInfoHeader);
	    	}	
    		else if(menu[0] == '7')
    		{
    			saveFile(bitmapData, &bitmapFileHeader, &bitmapInfoHeader);
    		}
    		else if(menu[0] == '8')
    		{
			freeBitmap(bitmapData, bitmapInfoHeader);
			return 0;
		}
		else
		{
			fputs("Krytyczny błąd programu.\n", stderr);
			exit(EXIT_FAILURE);
		}
	}
}
