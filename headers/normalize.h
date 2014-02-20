#ifndef HEADER
	#include "headers/pgmp2->h"
#endif

#include <omp.h>
#include <time.h>

void normalize(pgmp2_t* img, int min, int max){
	double factor = (float)(max - min) / (float)(img->max - img->min);

	clock_t t = clock();
	int pixel, new_pixel, i;
	int length = img->width * img->height;
	int chunki = (img->height) / omp_get_max_threads();
	int chunkj = (img->width) / omp_get_max_threads();
	//#pragma omp parallel for schedule(static, chunki) shared(img) private(i, j, pixel, new_pixel)
		for(i = 0; i < length; i++){
			pixel = get(*img, i);
			new_pixel = (pixel - img->min) * factor + min;
			set(*img, i, new_pixel);
		}

	img->min = min;
	img->max = max;

	t = clock() - t;
	printf("Normalization: %fs\n", ((float)t) / CLOCKS_PER_SEC);
}