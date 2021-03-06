#ifndef _CORE_H
#define _CORE_H
#include <syslog.h>
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
	int* receive_buffer;

	//relevant only for root process
	int* buffer_sizes;
	int* offsets;
} mpi_env_t;

extern mpi_env_t get_mpi_env(int numtasks, int root, int rank, int length);

extern void normalize(int old_min, int old_max, int new_min, int new_max, mpi_env_t env);

extern int get_max(mpi_env_t env);

extern int get_min(mpi_env_t env);

extern void scatter_pixels(int* pixels, mpi_env_t env);

extern void gather_pixels(int* pixels, mpi_env_t env);

#endif