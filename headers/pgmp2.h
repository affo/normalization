#include <malloc.h>

typedef struct pgmp2 {
	int min;
	int max;
	int width;
	int height;
	int* pixels;
} pgmp2_t;

pgmp2_t init(int width, int height){
	pgmp2_t* img = (pgmp2_t*) malloc(sizeof(pgmp2_t));
	img->width = width;
	img->height = height;

	//allocating memory for the matrix
	img->pixels = (int*) malloc(sizeof(int)*width*height);

	return *img;
}

int get(pgmp2_t img, int i){
	return img.pixels[i];
}

void set(pgmp2_t img, int i, int value){
	img.pixels[i] = value;
}