#ifndef _CORE_H
#define _CORE_H
#include "pgmp2.h"
#include "mpi.h"
#include "omp.h"
#ifndef __APPLE__
	#include <malloc.h>
#else
	#include <stdlib.h>
#endif

typedef struct mpi_env {
	int numtasks;
	int rank;
	int root;
	int chunk;
	int* buffer_sizes;
	int* offsets;
	int* receive_buffer;
} mpi_env_t;

extern mpi_env_t get_mpi_env(int numtasks, int root, int rank, int length);

extern void normalize(pgmp2_t* img, int new_min, int new_max, mpi_env_t env);

extern int get_max(pgmp2_t img, mpi_env_t env);

extern int get_min(pgmp2_t img, mpi_env_t env);

extern void scatter_pixels(pgmp2_t img, mpi_env_t env);

extern void gather_pixels(pgmp2_t* img, mpi_env_t env);

#endif