final.o: main.o filters.o bitmap.o
	gcc build/main.o build/filters.o build/bitmap.o -o bin/final.o

main.o:
	gcc main.c -c -Wall -pedantic -o build/main.o

filters.o:
	gcc filters.c -c -Wall -pedantic -o build/filters.o

bitmap.o:
	gcc bitmap.c -c -Wall -pedantic -o build/bitmap.o