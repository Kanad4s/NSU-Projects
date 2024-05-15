#include <iostream>
#include <cmath>
//#include <math.h>
#include "mpi.h"

#define epsilon 1e-8
#define alpha 1e+5

#define Nx 16
#define Ny 16
#define Nz 16

#define Dx 2
#define Dy 2
#define Dz 2

#define X0 -1
#define Y0 -1
#define Z0 -1

#define Hx (Dx / (Nx - 1))
#define Hy (Dy / (Ny - 1))
#define Hz (Dz / (Nz - 1))

#define factor (1.0 / ((2.0 / (Hx * Hx)) + (2.0 / (Hy * Hy)) + (2.0 / (Hz * Hz)) + alpha))

double phi(double x, double y, double z) {
    return x * x + y * y + z * z;
}

double roFunction(double x, double y, double z) {
    return 6 - alpha * phi(x, y, z);
}

double newCoor(double startCoor, double ort, double step) {
    return startCoor + ort * step;
}

void setFunctionValuesInRegion(double* functionValues, double* nextFunctionValues, int layerHeight, int rank, int nProcesses) {
    for (int k = 0; k < layerHeight; k++) {
        for (int j = 0; j < Ny; j++) {
            for (int i = 0; i < Nx; i++) {
                if (i == 0 || j == 0 || i == Nx - 1 || j == Ny - 1) {
                    functionValues[k * Ny * Nx + j * Nx + i] = phi(newCoor(X0, i, Hx), newCoor(Y0, j, Hy), 
                        newCoor(Z0, k + layerHeight * rank, Hz));
                }
                else {
                    functionValues[k * Ny * Nx + j * Nx + i] = 0;
                }
                nextFunctionValues[k * Ny * Nx + j * Nx + i] = functionValues[k * Ny * Nx + j * Nx + i];
            }
        }
    }
    if (rank == 0) {
        for (int j = 0; j < Ny; j++) {
            for (int i = 0; i < Nx; i++) {
                functionValues[j * Nx + i] = phi(newCoor(X0, i, Hx), newCoor(Y0, j, Hy), Z0);
                nextFunctionValues[j * Nx + i] = functionValues[j * Nx + i];
            }
        }
    }
    if (rank == nProcesses - 1) {
        for (int j = 0; j < Ny; j++) {
            for (int i = 0; i < Nx; i++) {
                functionValues[(layerHeight - 1) * Ny * Nx + j * Nx + i] = phi(newCoor(X0, i, Hx), newCoor(Y0, j, Hy), Z0 + Dz);
                nextFunctionValues[(layerHeight - 1) * Ny * Nx + j * Nx + i] = functionValues[(layerHeight - 1) * Ny * Nx + j * Nx + i];
            }
        }
    }
}

double calculateNextInnerValue(double* functionValues, int x, int y, int z) {
    double xSum = (functionValues[z * Ny * Nx + y * Nx + x + 1] + functionValues[z * Ny * Nx + y * Nx + x + 1]) / (Hx * Hx);
    double ySum = (functionValues[z * Ny * Nx + (y + 1) * Nx + x] + functionValues[z * Ny * Nx + (y - 1) * Nx + x]) / (Hy * Hy);
    double zSum = (functionValues[(z + 1) * Ny * Nx + y * Nx + x] + functionValues[(z - 1) * Ny * Nx + y * Nx + x]) / (Hz * Hz);
    double resultSum = xSum + ySum + zSum - roFunction(newCoor(X0, x, Hx), newCoor(Y0, y, Hy), newCoor(Z0, z, Hz));
    return factor * resultSum;
}

double calculateNextDownBoundValue(double* functionValues, double* downLayer, int x, int y, int layerHeight, int rank) {
    double xSum = (functionValues[y * Nx + x + 1] + functionValues[y * Nx + x + 1]) / (Hx * Hx);
    double ySum = (functionValues[(y + 1) * Nx + x] + functionValues[(y - 1) * Nx + x]) / (Hy * Hy);
    double zSum = (functionValues[Ny * Nx + y * Nx + x] + downLayer[y * Nx + x]) / (Hz * Hz);
    double resultSum = xSum + ySum + zSum - roFunction(newCoor(X0, x, Hx), newCoor(Y0, y, Hy), newCoor(Z0, layerHeight * rank, Hz));
    return factor * resultSum;
}

double calculateNextTopBoundValue(double* functionValues, double* topLayer, int x, int y, int layerHeight, int rank) {
    double xSum = (functionValues[(layerHeight - 1) * Ny * Nx + y * Nx + x + 1] +
                   functionValues[(layerHeight - 1) * Ny * Nx + y * Nx + x - 1]) / (Hx * Hx);
    double ySum = (functionValues[(layerHeight - 1) * Ny * Nx + (y + 1) * Nx + x] + 
                   functionValues[(layerHeight - 1) * Ny * Nx + (y - 1) * Nx + x]) / (Hy * Hy);
    double zSum = (topLayer[y * Nx + x] + functionValues[(layerHeight - 2) * Ny * Nx + y * Nx + x]) / (Hz * Hz);
    double resultSum = xSum + ySum + zSum - roFunction(newCoor(X0, x, Hx), newCoor(Y0, y, Hy), newCoor(Z0, (layerHeight + 1) * rank - 1, Hz));
    return factor * resultSum;
}

bool isAccuracyAchieved(int accuracy) {
    int isAchieved;
    MPI_Allreduce(&accuracy, &isAchieved, 1, MPI_INTEGER, MPI_SUM, MPI_COMM_WORLD);
    return isAchieved == 0;
}

void computeFunction(double* functionValues, double* nextFunctionValues, int layerHeight, int rank, int nProcesses) {
    MPI_Request requests[4];
    double* downLayer = (double*)malloc(sizeof(double) * Nx * Ny);
    double* topLayer = (double*)malloc(sizeof(double) * Nx * Ny);
    int accuracy = 1;
    while (isAccuracyAchieved(accuracy)) {
        accuracy = 0;
        memcpy(functionValues, nextFunctionValues, sizeof(double) * Nx * Ny * layerHeight);
        if (rank != 0) {
            MPI_Isend(&functionValues[0], Nx * Ny, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD, &requests[0]);
            MPI_Irecv(downLayer, Nx * Ny, MPI_DOUBLE, rank - 1, 2, MPI_COMM_WORLD, &requests[1]);
        }
        if (rank != nProcesses - 1) {
            MPI_Isend(&functionValues[(layerHeight - 1) * Nx * Ny], Nx * Ny, MPI_DOUBLE, rank + 1, 2, MPI_COMM_WORLD, &requests[2]);
            MPI_Irecv(topLayer, Nx * Ny, MPI_DOUBLE, rank + 1, 1, MPI_COMM_WORLD, &requests[3]);
        }
        for (int z = 1; z < layerHeight - 1; z++) {
            for (int y = 1; y < Ny - 1; y++) {
                for (int x = 1; x < Nx - 1; x++) {
                    nextFunctionValues[z * Ny * Nx + y * Ny + x] = calculateNextInnerValue(functionValues, x, y, z);
                    if (fabs(nextFunctionValues[z * Ny * Nx + y * Ny + x] - functionValues[z * Ny * Nx + y * Ny + x]) > epsilon) {
                        accuracy = 1;
                    }
                }
            }
        }
        if (rank != 0) {
            MPI_Wait(&requests[0], MPI_STATUS_IGNORE);
            MPI_Wait(&requests[1], MPI_STATUS_IGNORE);
        }
        if (rank != nProcesses - 1) {
            MPI_Wait(&requests[2], MPI_STATUS_IGNORE);
            MPI_Wait(&requests[3], MPI_STATUS_IGNORE);
        }

        for (int y = 1; y < Ny - 1; ++y) {
            for (int x = 1; x < Nx - 1; ++x) {
                if (rank != 0) {
                    nextFunctionValues[y * Nx + x] = calculateNextDownBoundValue(functionValues, downLayer, x, y, layerHeight, rank);
                }
                if (rank != nProcesses - 1) {
                    nextFunctionValues[(layerHeight - 1) * Ny * Nx + y * Nx + x] = calculateNextTopBoundValue(functionValues, topLayer, 
                        x, y, layerHeight, rank);
                }
                if (fabs(nextFunctionValues[Nx * y + x] - functionValues[Nx * y + x]) > epsilon) {
                    accuracy = 1;
                }
            }
        }
    }
    free(downLayer);
    free(topLayer);
}

void jacobiMethod(int rank, int nProcesses) {
    int layerHeight = Nz / nProcesses;
    double* functionValues = (double*)malloc(sizeof(double) * Nx * Ny * layerHeight);
    double* nextFunctionValues = (double*)malloc(sizeof(double) * Nx * Ny * layerHeight);
    setFunctionValuesInRegion(functionValues, nextFunctionValues, layerHeight, rank, nProcesses);
    computeFunction(functionValues, nextFunctionValues, layerHeight, rank, nProcesses);
    free(functionValues);
    free(nextFunctionValues);
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
