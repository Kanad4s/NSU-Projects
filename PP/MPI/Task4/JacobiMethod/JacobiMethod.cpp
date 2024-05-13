#include <iostream>
#include "mpi.h"

#define epsilon 1e-8
#define alpha 1e+5

enum {
    Nx = 16,
    Ny = 16,
    Nz = 16,
    Dx = 2,
    Dy = 2,
    Dz = 2,
    X0 = -1,
    Y0 = -1,
    Z0 = -1,
    Hx = Dx / (Nx - 1),
    Hy = Dy / (Ny - 1),
    Hz = Dz / (Nz - 1)
};

double phi(double x, double y, double z) {
    return x * x + y * y + z * z;
}

double ro(double x, double y, double z) {
    return 6 - alpha * phi(x, y, z);
}

double newCoor(double startCoor, double ort, double step) {
    return startCoor + ort * step;
}

void setFunctionValuesInRegion(int layerHeight) {
    for (int k = 0; k < layerHeight; k++) {
        for (int j = 0; j < Ny; j++) {
            for (int i = 0; i < Nx; i++) {
                if (i == 0 || j == 0 || i == Nx - 1 || j == Ny - 1) {

                }
                else {

                }
            }
        }
    }
    
}

void jacobiMethod(int rank, int nProcesses) {
    int layerHeight = Nz / nProcesses;
    setFunctionValuesInRegion(layerHeight);
}


int main(int argc, char* argv[])
{
    int rank;
    int nProcesses;
    MPI_Init(&argc, &argv);
    printf("%f\n", alpha);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
    double start = MPI_Wtime();
    jacobiMethod(rank, nProcesses);
    double finish = MPI_Wtime();
    if (rank == 0) {
        printf("TIME: %f", finish - start);
    }

    MPI_Finalize();
    return 0;
}
