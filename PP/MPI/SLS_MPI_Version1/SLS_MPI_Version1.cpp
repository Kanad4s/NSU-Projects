#include <iostream>
#include "mpi.h"

int main(int argc, char** argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Hello World! proc:%d\n", rank);
    MPI_Finalize();
    std::cout << "process: " << rank << " size: " << size << std::endl;
    //std::cout << "End\n";
}