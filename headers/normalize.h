#ifndef HEADER
	#include "headers/pgmp2.h"
#endif

pgmp2_t normalize(pgmp2_t img, int min, int max){
	int factor = (max - min) / (img.max - img.min);

	int pixel, new_pixel, i, j;
	for(i = 0; i < img.height; i++){
		for(j = 0; j < img.width; j++){
			pixel = get(img, i, j);
			new_pixel = (pixel - img.min) * factor + min;
			set(img, i, j, new_pixel);
		}
	}

	img.min = min;
	img.max = max;

	return img;
}