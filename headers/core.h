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

mpi_env_t get_mpi_env(int numtasks, int root, int rank, int length){
	mpi_env_t* env = (mpi_env_t*) malloc(sizeof(mpi_env_t));
	//if i'm the last one
	env->numtasks = numtasks;
	env->root = root;
	env->rank = rank;
	int chunk = length / numtasks;
	env->chunk = chunk;
	if(rank == (numtasks - 1)){
		chunk = length - (chunk * (numtasks - 1));
	}
	printf("rank: %d, chunk: %d\n", rank, chunk);
		
	env->receive_buffer = (int*) malloc(sizeof(int)*chunk);
	

	if(rank == 0){
		env->offsets = (int*) malloc(sizeof(int)*numtasks);
		env->buffer_sizes = (int*) malloc(sizeof(int)*numtasks);
		for(int i = 0; i < numtasks; i++){
		  	env->offsets[i] = i * chunk;
		}
		for(int i = 0; i < (numtasks - 1); i++){
			env->buffer_sizes[i] = chunk;
		}
		env->buffer_sizes[numtasks - 1] = length - (chunk * (numtasks - 1));
	}

	return *env;
}


void normalize(pgmp2_t* img, int new_min, int new_max, mpi_env_t env){
	int i;
	int length = img->width * img->height;
	int min = img->min;
	int max = img->max;
	int* pixels = img->pixels;
	double factor = (float)(new_max - new_min) / (float)(max - min);

	int* buffer_sizes = env.buffer_sizes;
	int* offsets = env.offsets;
	int* receive_buffer = env.receive_buffer;
	int chunk = env.chunk;
	int root = env.root;

	clock_t t = clock();
	MPI_Scatterv(pixels, buffer_sizes, offsets, MPI_INT, receive_buffer, chunk, MPI_INT, root, MPI_COMM_WORLD);

	//#pragma omp parallel for shared(recvbuf) private(i) firstprivate(new_min, factor, min)
		for(i = 0; i < chunk; i++){
			receive_buffer[i] = (receive_buffer[i] - min) * factor + new_min;
		}

	MPI_Gatherv(receive_buffer, chunk, MPI_INT, pixels, buffer_sizes, offsets, MPI_INT, root, MPI_COMM_WORLD);

	t = clock() - t;
	printf("Normalization: %fs\n", ((float)t) / CLOCKS_PER_SEC);

	img->min = new_min;
	img->max = new_max;
}

int get_max(pgmp2_t img, mpi_env_t env){
	int* pixels = img.pixels;
	int length = img.width * img.height;

	int* buffer_sizes = env.buffer_sizes;
	int* offsets = env.offsets;
	int* receive_buffer = env.receive_buffer;
	int chunk = env.chunk;
	int root = env.root;

	int global_max = 0;
	int local_max = 0;

	MPI_Scatterv(pixels, buffer_sizes, offsets, MPI_INT, receive_buffer, chunk, MPI_INT, root, MPI_COMM_WORLD);

	#pragma omp parallel for
		for (int i = 0; i < length; i++) {
  			#pragma omp critical
			 	if (pixels[i] > local_max) local_max = pixels[i];
		}

	MPI_Allreduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

	return global_max;
}

int get_min(pgmp2_t img, mpi_env_t env){
	int* pixels = img.pixels;
	int length = img.width * img.height;

	int* buffer_sizes = env.buffer_sizes;
	int* offsets = env.offsets;
	int* receive_buffer = env.receive_buffer;
	int chunk = env.chunk;
	int root = env.root;

	int global_min = 255;
	int local_min = 255;

	MPI_Scatterv(pixels, buffer_sizes, offsets, MPI_INT, receive_buffer, chunk, MPI_INT, root, MPI_COMM_WORLD);

	#pragma omp parallel for
		for (int i = 0; i < length; i++) {
  			#pragma omp critical
			 	if (pixels[i] < local_min) local_min = pixels[i];
		}

	MPI_Allreduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
	

	return global_min;
}