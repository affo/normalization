#include <stdio.h>
#include <stdlib.h>

void usage(){
	printf("norm <input-file> <new-min> <new-max>\n");
}

int normalize(int pixel, int min, int max, int new_min, int new_max){
	int norm = 0;
	int factor = (new_max - new_min) / (max - min);
	norm = (pixel - min) * factor + new_min;
	return norm;
}

int main(int argc, char** argv){
	FILE* in = NULL;
	FILE* out = NULL;

	if(argc < 4){
		usage();
		exit(-1);
	}

	int new_min = atoi(argv[2]);
	int new_max = atoi(argv[3]);

	in = fopen(argv[1], "r");
	out = fopen("out.pgm", "w");

	if(in == NULL || out == NULL){
		printf("Error in opening file\n");
		exit(-1);
	}

	char* magic = (char*) malloc(sizeof(char)*2);
	int width;
	int height;
	int max;
	fscanf(in, "%s%*c", magic);
	fscanf(in, "%d%*c", &width);
	fscanf(in, "%d%*c", &height);
	fscanf(in, "%d%*c", &max);

	fprintf(out, "%s %d %d %d\n", magic, width, height, new_max);

	int pixel;
	char separator;
	while(!feof(in)){
		fscanf(in, "%d%c", &pixel, &separator);
		pixel = normalize(pixel, 0, max, new_min, new_max);
		fprintf(out, "%d%c", pixel, separator);
	}

	return 0;
}