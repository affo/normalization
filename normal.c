#ifndef HEADER
#define HEADER
	#include "headers/pgmp2.h"
	#include "headers/utils.h"
	#include "headers/normalize.h"
#endif

void usage(){
	printf("norm <input-file> <new-min> <new-max>\n");
}

int main(int argc, char** argv){
	if(argc < 4){
		usage();
		exit(-1);
	}

	int new_min = atoi(argv[2]);
	int new_max = atoi(argv[3]);

	pgmp2_t img = load(argv[1]);
	normalize(&img, new_min, new_max);
	store(img);
}