#include "../headers/pgmp2.h"

pgmp2_t init(int width, int height){
	pgmp2_t* img = (pgmp2_t*) malloc(sizeof(pgmp2_t));
	img->width = width;
	img->height = height;

	img->pixels = (int*) malloc(sizeof(int)*width*height);

	return *img;
}

int get(pgmp2_t img, int i){
	return img.pixels[i];
}

void set(pgmp2_t img, int i, int value){
	img.pixels[i] = value;
}