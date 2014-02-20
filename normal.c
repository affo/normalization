#ifndef HEADER
#define HEADER
	#include "headers/pgmp2.h"
	#include "headers/utils.h"
	#include "headers/core.h"
	#include <mpi.h>
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
	int length = img.width * img.height;
	int* pixels = img.pixels;

	int numtasks, rank, root;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	int max = get_max(img, numtasks, root);
	int min = get_min(img, numtasks, root);

	normalize(&img, new_min, new_max, numtasks, root);

	MPI_Finalize();
	store(img);
}