#include "headers/utils.h"

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
	/*if(argc < 4){
		usage();
		exit(-1);
	}

	int new_min = atoi(argv[2]);
	int new_max = atoi(argv[3]);*/


	pgmp2_t img = load(argv[1]);
	store(img);
}