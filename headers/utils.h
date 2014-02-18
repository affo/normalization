#include <stdio.h>
#include <stdlib.h>
#ifndef HEADER
	#include "headers/pgmp2.h"
#endif

#define MAX_LINE_LENGTH 40

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
	fscanf(in, "%s", magic);
	if(strcmp(magic, "P2")){
		printf("Incorrect type of image, need a P2 image\n");
		fclose(in);
		exit(-1);
	}

	fscanf(in, "%d", &width);
	fscanf(in, "%d", &height);
	fscanf(in, "%d", &max);

	pgmp2_t img = init(width, height, max);
	int pixel, i, j;
	int min = 255;
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			if(feof(in)) break;

			fscanf(in, "%d", &pixel);
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
	int newline = img.width > MAX_LINE_LENGTH;
	int pixel_count = 1;

	for(i = 0; i < img.height; i++){
		for(j = 0; j < img.width; j++, pixel_count++){
			fprintf(out, "%d ", get(img, i, j));
			if(newline && (pixel_count%MAX_LINE_LENGTH == 0)){
				fprintf(out, "\n");
				pixel_count = 0;
			}
		}

		if(!newline){
			fprintf(out, "\n");
		}
	}

	fclose(out);
}