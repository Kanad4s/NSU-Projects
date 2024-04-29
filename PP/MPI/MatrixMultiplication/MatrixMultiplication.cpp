#include <iostream>
#include "mpi.h"

const double N1 = 8;
const double N2 = 4;
const double N3 = 8;

enum {
    NUMBER_DIMS = 2,
    X = 0,
    Y = 1,
    ROOT = 0
};

void printVector(double* vector, int size, int rank)
{
    printf("rank:%d\n", rank);
    for (int i = 0; i < size; i++) {
        printf("%1.0f ", vector[i]);
    }
    printf("\n");
}

double* generateMatrixA(int nLines, int nColumns) {
    double* matrix = new double[nLines * nColumns];
    int addPart = 0;
    for (int i = 0; i < nLines; i++) {
        
        for (int j = 0; j < nColumns; j++) {
            matrix[i * nColumns + j] = j + 1 + addPart;
        }
        if (i != 0 && (i - 1) % 2 == 0) {
            addPart += nColumns;

        }
    }
    return matrix;
}

double* generateMatrixB(int nLines, int nColumns) {
    double* matrix = new double[nLines * nColumns];
    for (int i = 0; i < nLines; i++) {
        for (int j = 0; j < nColumns; j++) {
            matrix[i * nColumns + j] = j + 1;
        }
    }
    return matrix;
}

double* generateZeroVector(int vectorSize)
{
    double* vector = new double[vectorSize];
    for (int i = 0; i < vectorSize; i++) {
        vector[i] = 0;
    }
    return vector;
}

void createComms(int coords[], int dims[], int rank, int nProcesses, MPI_Comm* comm2d, MPI_Comm* commColumn, MPI_Comm* commLine) {
    MPI_Dims_create(nProcesses, NUMBER_DIMS, dims);
    int periods[NUMBER_DIMS] = { 0, 0 };
    int reorder = 1;
    MPI_Cart_create(MPI_COMM_WORLD, NUMBER_DIMS, dims, periods, reorder, comm2d);
    /*for (int i = 0; i < 2; i++) {
        printf("%d ", dims[i]);
    }*/
    MPI_Cart_coords(*comm2d, rank, NUMBER_DIMS, coords);
    int coordsLine[NUMBER_DIMS] = { 0, 1 };
    int coordsColumn[NUMBER_DIMS] = { 1, 0 };
    MPI_Cart_sub(*comm2d, coordsLine, commLine);
    MPI_Cart_sub(*comm2d, coordsColumn, commColumn);
}

void calculateSizes(int* partASize, int* partBSize, int n1, int n2, int n3, int dims[]) {
    *partASize = n1 * n2 / dims[X];
    *partBSize = n2 * n3 / dims[Y];
}

void distributeMatrixA(double* A, double* partA, int partASize, int coords[], MPI_Comm commColumn, MPI_Comm commLine) {
    if (coords[Y] == 0) {
        MPI_Scatter(A, partASize, MPI_DOUBLE, partA, partASize, MPI_DOUBLE, ROOT, commColumn);
    }
    MPI_Bcast(partA, partASize, MPI_DOUBLE, ROOT, commLine);
    //printVector(partA, partASize, 20);
}

void distributeMatrixB(double* B, double* partB, int partBSize, int coords[], int n2, int n3,int rank, MPI_Comm commColumn, MPI_Comm commLine) {
    if (coords[X] == 0) {
        MPI_Datatype vectorType;
        MPI_Type_vector(n2, partBSize / n2, n3, MPI_DOUBLE, &vectorType);
        //MPI_Type_commit(&vectorType);
        MPI_Type_create_resized(vectorType, 0, sizeof(double) * partBSize / n2, &vectorType);
        MPI_Type_commit(&vectorType);
        MPI_Scatter(B, 1, vectorType, partB, partBSize, MPI_DOUBLE, ROOT, commLine);
    }
    MPI_Bcast(partB, partBSize, MPI_DOUBLE, ROOT, commColumn);
    //printVector(partB, partBSize, rank);
}

double* multiplyPartMatrices(double* partA, double* partB, int n1, int n2, int n3, int coords[]) {

}

double* multiplyMatrices(double* A, double* B, int n1, int n2, int n3, int rank, int nProcesses) {
    int coords[NUMBER_DIMS] = { 0, 0 };
    int dims[NUMBER_DIMS] = { 0, 0 };
    MPI_Comm comm2d, commColumn, commLine;
    createComms(coords, dims, rank, nProcesses, &comm2d, &commColumn, &commLine);
    int partASize, partBsize;
    calculateSizes(&partASize, &partBsize, n1, n2, n3, dims);
    if (rank == 0) {
        for (int i = 0; i < 2; i++) {
            printf("%d ", dims[i]);
        }
        //printf("\n");
        //printf("%d\n", partASize);
        //printVector(A, n1 * n2, rank);
    }
    double* partA = generateZeroVector(partASize);
    double* partB = generateZeroVector(partBsize);
    distributeMatrixA(A, partA, partASize, coords, commColumn, commLine);
    distributeMatrixB(B, partB, partBsize, coords, n2, n3, rank, commColumn, commLine);
    //printVector(partA, partASize, rank);
    //distributeMatrices(A, B, partA, partB, partASize, partBsize, coords, rank, commColumn, commLine);
    /*printVector(A, n1 * n2, rank);
    printf("\nB\n");
    printVector(B, n2 * n3, rank);*/
    /*printf("\nrank:%d\n", rank);
    for (int i = 0; i < 2; i++) {
        printf("%d ", coords[i]);
    }*/
    double* result = new double[n1 * n3];
    return result;
}

int main(int argc, char* argv[])
{
    double* A = generateMatrixA(N1, N2);
    double* B = generateMatrixB(N2, N3);
    MPI_Init(&argc, &argv);
    double start = MPI_Wtime();
    int rank, nProcesses;
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double* solution = multiplyMatrices(A, B, N1, N2, N3, rank, nProcesses);
    double finish = MPI_Wtime();
    if (rank == 0) {
        printf("Time: %f\n", finish - start);
    }
    delete[] A;
    delete[] B;
    delete[] solution;
    MPI_Finalize();
    return 0;
}