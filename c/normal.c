#include "../headers/pgmp2.h"
#include "../headers/utils.h"
#include "../headers/core.h"
#include <time.h>
#include <stdio.h>

void usage(){
	printf("norm <input-file> <new-min> <new-max>\n");
}

void print_time(char* str, clock_t t, int rank){
	if(rank == 0){
		t = clock() - t;
		printf("%s: %fs\n", str, ((float)t) / CLOCKS_PER_SEC);
	}
}

int main(int argc, char** argv){
	if(argc < 4){
		usage();
		exit(-1);
	}
	
	int numtasks, rank;
	clock_t t;
	pgmp2_t img;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	
	int length, old_min, old_max;
	int new_min = atoi(argv[2]);
	int new_max = atoi(argv[3]);
	if(rank == 0){
		t = clock();
		img = load(argv[1]);
		print_time("Loading", t, rank);
		length = img.width * img.height;
		img.min = new_min;
		img.max = new_max;
	}

	MPI_Bcast(&length, 1, MPI_INT, 0, MPI_COMM_WORLD);
	mpi_env_t env = get_mpi_env(numtasks, 0, rank, length);

	scatter_pixels(img.pixels, env);

	t = clock();
	old_max = get_max(env);
	print_time("Getting MAX", t, rank);

	t = clock();
	old_min = get_min(env);
	print_time("Getting MIN", t, rank);

	t = clock();
	normalize(old_min, old_max, new_min, new_max, env);
	gather_pixels(img.pixels, env);
	print_time("Normalizing", t, rank);

	if(rank == 0){
		t = clock();
		store(img);
		print_time("Storing", t, rank);
	}
	MPI_Finalize();
}