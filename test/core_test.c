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
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	root = 0;

	int max = get_max(img, numtasks, root);
	int min = get_min(img, numtasks, root);

	MPI_Finalize();
	return 0;
}