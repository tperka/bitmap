#ifndef FILTERS_H
#define FILTERS_H
#include "bitmap.h"

pixel_24bit average(pixel_24bit **matrix, int sizev, int sizeh);

pixel_24bit minimal(pixel_24bit **matrix, int sizev, int sizeh);

pixel_24bit max(pixel_24bit **matrix, int sizev, int sizeh);

pixel_24bit median(pixel_24bit **matrix, int sizev, int sizeh);


#endif