#ifndef HEADER
	#include "pgmp2.h"
	#include "mpi.h"
#endif

#include <omp.h>
#include <time.h>
#include "mpi.h"

typedef struct mpi_env {
	int numtasks;
	int rank;
	int root;
	int chunk;
	int* buffer_sizes;
	int* offsets;
	int* receive_buffer;
} mpi_env_t;

mpi_env_t get_mpi_env(int, int, int, int);


void normalize(pgmp2_t, int, int, mpi_env_t);

int get_max(pgmp2_t, mpi_env_t);

int get_min(pgmp2_t, mpi_env_t);

void scatter_pixels(pgmp2_t, mpi_env_t);

void gather_pixels(pgmp2_t, mpi_env_t);