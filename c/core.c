#include "../headers/core.h"

mpi_env_t get_mpi_env(int numtasks, int root, int rank, int length){
	mpi_env_t* env = (mpi_env_t*) malloc(sizeof(mpi_env_t));
	//if i'm the last one
	env->numtasks = numtasks;
	env->root = root;
	env->rank = rank;
	int chunk = length / numtasks;
	if(rank == (numtasks - 1)){
		chunk = length - (chunk * (numtasks - 1));
	}
	env->chunk = chunk;
		
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

void normalize(int old_min, int old_max, int new_min, int new_max, mpi_env_t env){
	double factor = (float)(new_max - new_min) / (float)(old_max - old_min);

	int* receive_buffer = env.receive_buffer;
	int chunk = env.chunk;

	int i;
	int chunk_size = chunk / omp_get_max_threads();
	#pragma omp parallel for \
		schedule(static, chunk_size) \
		shared(receive_buffer) \
		private(i) \
		firstprivate(new_min, factor, old_min)
		for(i = 0; i < chunk; i++){
			receive_buffer[i] = (receive_buffer[i] - old_min) * factor + new_min;
		}
}

int get_max(mpi_env_t env){
	int* receive_buffer = env.receive_buffer;
	int chunk = env.chunk;

	int global_max = 0;
	int local_max = 0;

	#pragma omp parallel shared(receive_buffer, local_max)
	{
		int submax = 0, i;		
		#pragma omp for
			for (i = 0; i < chunk; i++) {
			 	if (receive_buffer[i] > submax) submax = receive_buffer[i];
			}

		#pragma omp critical
			if(submax > local_max) local_max = submax;
	}

	MPI_Allreduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

	return global_max;
}

int get_min(mpi_env_t env){
	int* receive_buffer = env.receive_buffer;
	int chunk = env.chunk;

	int global_min = 255;
	int local_min = 255;

	#pragma omp parallel shared(receive_buffer, local_min)
	{
		int submin = 255, i;		
		#pragma omp for
			for (i = 0; i < chunk; i++) {
			 	if (receive_buffer[i] < submin) submin = receive_buffer[i];
			}

		#pragma omp critical
			if(submin < local_min) local_min = submin;
	}

	MPI_Allreduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
	
	return global_min;
}

void scatter_pixels(int* pixels, mpi_env_t env){
	int* buffer_sizes = env.buffer_sizes;
	int* offsets = env.offsets;
	int* receive_buffer = env.receive_buffer;
	int chunk = env.chunk;
	int root = env.root;

	MPI_Scatterv(pixels, buffer_sizes, offsets, MPI_INT, receive_buffer, chunk, MPI_INT, root, MPI_COMM_WORLD);
}

void gather_pixels(int* pixels, mpi_env_t env){
	int* buffer_sizes = env.buffer_sizes;
	int* offsets = env.offsets;
	int* receive_buffer = env.receive_buffer;
	int chunk = env.chunk;
	int root = env.root;

	MPI_Gatherv(receive_buffer, chunk, MPI_INT, pixels, buffer_sizes, offsets, MPI_INT, root, MPI_COMM_WORLD);
}