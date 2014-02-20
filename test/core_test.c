#include <stdio.h>
#ifndef HEADER
#define HEADER
	#include "../headers/pgmp2.h"
	#include "../headers/utils.h"
	#include "../headers/core.h"
	#include "mpi.h"
#endif


int main(int argc, char** argv){

	pgmp2_t img = load("../images/feep.pgm");

	int numtasks, rank, root;
	

	int max = get_max(img);
	int min = get_min(img);

	// MPI_Init(&argc,&argv);
	// MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	// root = 0;

	// MPI_Finalize();

	printf("max: %d, min: %d\n", max, min);
	return 0;
}