#include <malloc.h>

typedef struct pgmp2 {
	int min;
	int max;
	int width;
	int height;
	int** pixels;
} pgmp2_t;

pgmp2_t init(int width, int height, int max){
	pgmp2_t* img = (pgmp2_t*) malloc(sizeof(pgmp2_t));
	img->width = width;
	img->height = height;
	img->max = max;

	//allocating memory for the matrix
	int i;
	img->pixels = (int**) malloc(sizeof(int*)*height);
	for(i = 0; i < height; i++){
		img->pixels[i] = (int*) malloc(sizeof(int)*width);
	}

	return *img;
}

int get(pgmp2_t img, int i, int j){
	return img.pixels[i][j];
}

void set(pgmp2_t img, int i, int j, int value){
	img.pixels[i][j] = value;
}