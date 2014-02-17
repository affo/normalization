#include <stdio.h>
#include <stdlib.h>
#ifndef HEADER
	#include "headers/pgmp2.h"
#endif

pgmp2_t load(char* filename){
	FILE* in = NULL;
	in = fopen(filename, "r");

	if(in == NULL){
		printf("Error in opening file\n");
		exit(-1);
	}
	char* magic = (char*) malloc(sizeof(char)*2);
	int width;
	int height;
	int max;
	fscanf(in, "%s%*c", magic);
	if(strcmp(magic, "P2")){
		printf("Incorrect type of image, need a P2 image\n");
		fclose(in);
		exit(-1);
	}

	fscanf(in, "%d%*c", &width);
	fscanf(in, "%d%*c", &height);
	fscanf(in, "%d%*c", &max);

	pgmp2_t img = init(width, height, max);
	int pixel, i, j;
	int min = 255;
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			if(feof(in)) break;

			fscanf(in, "%d%*c", &pixel);
			set(img, i, j, pixel);
			if(pixel < min){
				min = pixel;
			}
		}
	}

	img.min = min;
	fclose(in);

	return img;
}

void store(pgmp2_t img){
	FILE* out = NULL;
	out = fopen("out.pgm", "w");

	if(out == NULL){
		printf("Error in creating file\n");
		exit(-1);
	}

	fprintf(out, "P2\n");
	fprintf(out, "%d %d\n", img.width, img.height);
	fprintf(out, "%d\n", img.max);

	int i, j;

	for(i = 0; i < img.height; i++){
		for(j = 0; j < img.width; j++){
			fprintf(out, "%d ", get(img, i, j));
		}
		fseek(out, -1, SEEK_CUR);
		fprintf(out, "\n");
	}

	fclose(out);
}