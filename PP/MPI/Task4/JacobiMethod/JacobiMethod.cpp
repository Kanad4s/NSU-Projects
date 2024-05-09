#include <iostream>
#include "mpi.h"

#define epsilon 10e-9
#define a 10e5


enum {
    Nx = 20,
    Ny = 20,
    Nz = 20,
    Dx = 2,
    Dy = 2,
    Dz = 2,
    X0 = -1,
    Y0 = -1,
    Z0 = -1
};

void generateOffsetVector() {

}

void jacobiMethod(int rank, int nProcesses) {
    int layerHeight = Nz / nProcesses;
}


int main(int argc, char* argv[])
{
    int rank;
    int nProcesses;
    MPI_Init(&argc, &argv);

    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
    double start = MPI_Wtime();
    jacobiMethod(rank, nProcesses);
    double finish = MPI_Wtime();
    if (rank == 0) {
        printf("TIME: %d", finish - start);
    }

    MPI_Finalize();
    return 0;
}
