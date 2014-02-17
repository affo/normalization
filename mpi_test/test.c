#include "mpi.h"
#include <string.h>
#include <stdio.h>

#define DIM 20

int main(int argc, char *argv[]) {
    int rank, size, dest, source;
    int tag = 1;
    char in[DIM], out[DIM];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0){
    	dest = 1; source = 1;
    	while(1){
    		scanf("%s", out);
    		MPI_Send(out, DIM, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    		MPI_Recv(in, DIM, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
    		printf("%s\n", in);
    	}

    }else if(rank == 1){
    	dest = 0; source = 0;
    	while(1){
    		MPI_Recv(in, DIM, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
    		printf("%s\n", in);
    		MPI_Send(in, DIM, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    	}
    }

    MPI_Finalize();
    return 0;
}