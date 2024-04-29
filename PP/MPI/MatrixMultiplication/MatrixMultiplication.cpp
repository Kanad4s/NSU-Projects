#include <iostream>
#include "mpi.h"

const double N1 = 4;
const double N2 = 6;
const double N3 = 4;

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
            matrix[i * nColumns + j] = j + 1 + i * nColumns;
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
            matrix[i * nColumns + j] = j + 1 + 24 + i * nColumns;
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
}

void distributeMatrixB(double* B, double* partB, int partBSize, int coords[], int n2, int n3,int rank, MPI_Comm commColumn, MPI_Comm commLine) {
    if (coords[X] == 0) {
        MPI_Datatype vectorType;
        MPI_Type_vector(n2, partBSize / n2, n3, MPI_DOUBLE, &vectorType);
        MPI_Type_create_resized(vectorType, 0, sizeof(double) * partBSize / n2, &vectorType);
        MPI_Type_commit(&vectorType);
        MPI_Scatter(B, 1, vectorType, partB, partBSize, MPI_DOUBLE, ROOT, commLine);
    }
    MPI_Bcast(partB, partBSize, MPI_DOUBLE, ROOT, commColumn);
}

double* multiplyPartMatrices(double* partA, double* partB, int partASize, int partBSize, int n1, int n2, int n3, int coords[]) {
    double* result = generateZeroVector(n1 * n3);
    for (int i = 0; i < n1; i++) {
        for (int k = 0; k < n3; k++) {
            for (int j = 0; j < n2; j++) {
                result[i * n3 + k] += partA[i * n2 + j] * partB[j * n3 + k];
            }
        }
    }
    return result;
}

void CreateGathervOptions(int recvCounts[], int displs[], int dims[], int n1) {
    int sizeShift = n1 * dims[Y] / dims[X];
    for (int i = 0; i < dims[X]; ++i) {
        for (int j = 0; j < dims[Y]; ++j) {
            recvCounts[i * dims[Y] + j] = 1;
            displs[i * dims[Y] + j] = i * sizeShift + j;
        }
    }
}
void unitResultParts(double* result, double* partResult, int partResultSize, int nLinesPart, int nColumnsPart, int n1, int n3,
                     int dims[], int nProcesses, MPI_Comm* comm2d) {
    MPI_Datatype matrixPart;
    MPI_Type_vector(nLinesPart, nColumnsPart, n3, MPI_DOUBLE, &matrixPart);
    MPI_Type_commit(&matrixPart);
    MPI_Type_create_resized(matrixPart, 0, sizeof(double) * nColumnsPart, &matrixPart);
    MPI_Type_commit(&matrixPart);
    int* receiveCounts = new int[nProcesses];
    int* displays = new int[nProcesses];
    CreateGathervOptions(receiveCounts, displays, dims, n1);
    MPI_Gatherv(partResult, partResultSize, MPI_DOUBLE, result, receiveCounts, displays, matrixPart, ROOT, *comm2d);
    delete[] receiveCounts;
    delete[] displays;
}

double* multiplyMatrices(double* A, double* B, int n1, int n2, int n3, int rank, int nProcesses) {
    int coords[NUMBER_DIMS] = { 0, 0 };
    int dims[NUMBER_DIMS] = { 0, 0 };
    MPI_Comm comm2d, commColumn, commLine;
    createComms(coords, dims, rank, nProcesses, &comm2d, &commColumn, &commLine);
    int partASize, partBsize;
    calculateSizes(&partASize, &partBsize, n1, n2, n3, dims);
    double* partA = generateZeroVector(partASize);
    double* partB = generateZeroVector(partBsize);
    distributeMatrixA(A, partA, partASize, coords, commColumn, commLine);
    distributeMatrixB(B, partB, partBsize, coords, n2, n3, rank, commColumn, commLine);
    double* partResult = multiplyPartMatrices(partA, partB, partASize, partBsize, partASize / n2, n2, partBsize / n2, coords);
    int partResultSize = partASize / n2 * partBsize / n2;
    double* result = generateZeroVector(n1 * n3);
    unitResultParts(result, partResult, partResultSize, partASize / n2, partBsize / n2, n1, n3, dims, nProcesses, &comm2d);
    delete[] partA;
    delete[] partB;
    delete[] partResult;
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
        printVector(solution, N1 * N3, rank);
    }
    delete[] A;
    delete[] B;
    delete[] solution;
    MPI_Finalize();
    return 0;
}