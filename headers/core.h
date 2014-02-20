#ifndef HEADER
	#include "headers/pgmp2.h"
	#include <mpi.h>
#endif

#include <omp.h>
#include <time.h>

void normalize(pgmp2_t* img, int new_min, int new_max, int numtasks, int root){
	int i;
	int length = img->width * img->height;
	int min = img->min;
	int max = img->max
	int* pixels = img->pixels;
	double factor = (float)(new_max - new_min) / (float)(max - min);

	clock_t t = clock();

	int chunk = length / numtasks;
	int *recvbuf, *sendcounts, *offsets;
	//if you are the last one
	if(rank == (numtasks - 1)){
		chunk = length - (chunk * (numtasks - 1));
	}
	printf("rank: %d, chunk: %d\n", rank, chunk);

	recvbuf = (int*) malloc(sizeof(int)*chunk);

	if(rank == 0){
		offsets = (int*) malloc(sizeof(int)*numtasks);
		sendcounts = (int*) malloc(sizeof(int)*numtasks);
		for(int i = 0; i < numtasks; i++){
		  	offsets[i] = i * chunk;
		}
		for(int i = 0; i < (numtasks - 1); i++){
		  sendcounts[i] = chunk;
		}
		sendcounts[numtasks - 1] = length - (chunk * (numtasks - 1));
	}

	sendcount = chunk;
	recvcount = chunk;
	MPI_Scatterv(pixels, sendcounts, offsets, MPI_INT, recvbuf, recvcount, MPI_INT, root, MPI_COMM_WORLD);

	#pragma omp parallel for shared(recvbuf) private(i) firstprivate(new_min, factor, min)
		for(i = 0; i < chunk; i++){
			pixels[i] = (pixels[i] - min) * factor + new_min;
		}

	MPI_Gatherv(recvbuf, chunk, MPI_INT, pixels, sendcounts, offsets, MPI_INT, root, MPI_COMM_WORLD);

	t = clock() - t;
	printf("Normalization: %fs\n", ((float)t) / CLOCKS_PER_SEC);

	img->min = new_min;
	img->max = new_max;
}

int get_max(pgmp2_t img, int numtasks, int root){
	int* pixels = img.pixels;
	int length = img.width * img.height;
	int recv[numtasks];
	MPI_Reduce(pixels, recv, length, MPI_INT, MPI_MAX, root, MPI_COMM_WORLD);

	int max = 0;
	for(int i = 0; i < numtasks; i++){
		if(recv[i] > max){
			max = recv[i];
		}
	}

	return max;
}

int get_min(pgmp2_t img, int numtasks, int root){
	int* pixels = img.pixels;
	int length = img.width * img.height;
	int recv[numtasks];
	MPI_Reduce(pixels, recv, length, MPI_INT, MPI_MIN, root, MPI_COMM_WORLD);

	int min = 255;
	for(int i = 0; i < numtasks; i++){
		if(recv[i] < max){
			min = recv[i];
		}
	}

	return min;
}