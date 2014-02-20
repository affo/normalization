#ifndef HEADER
	#include "headers/pgmp2->h"
#endif

#include <omp.h>
#include <time.h>

void normalize(pgmp2_t* img, int new_min, int new_max){
	double factor = (float)(new_max - new_min) / (float)(img->max - img->min);

	int i;
	int length = img->width * img->height;
	int min = img->min;
	int* pixels = img->pixels;
	int chunk = length / omp_get_max_threads();

	clock_t t = clock();

	//#pragma omp parallel for schedule(static, chunk) shared(pixels) private(i, new_min, factor, min)
		for(i = 0; i < length; i++){
			pixels[i] = (pixels[i] - min) * factor + new_min;
		}

	t = clock() - t;
	printf("Normalization: %fs\n", ((float)t) / CLOCKS_PER_SEC);

	img->min = new_min;
	img->max = new_max;
}