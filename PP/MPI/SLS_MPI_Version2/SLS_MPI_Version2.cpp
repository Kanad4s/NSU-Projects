#include "mpi.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <math.h>

enum {
    SIZE_VECTOR = 8,//4096,
    ZERO_VALUE = 0,
    ROOT = 0
};

const double TAU = 1e-5;
const double EPSILON = 1e-5;

void printVector(double* vector, int size, int rank)
{
    printf("rank:%d\n", rank);
    for (int i = 0; i < size; i++) {
        printf("%1.0f ", vector[i]);
    }
    printf("\n");
}

void printPartMatrix(double* matrix, int partSize, int shift, int matrixSideSize,
    int nProcesses, int rank)
{
    for (int i = 0; i < nProcesses; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (i == rank) {
            //printf("rank = %d\n", rank);
            //printVector(partMatrix, partMatrixSize);
            printf("\n");
            //free space
            for (int i = 0; i < shift % matrixSideSize; i++) {
                printf(" ");
            }
            //part of matrix
            for (int i = 0; i < partSize; i++) {
                if (i != 0 && (i + shift) % matrixSideSize == 0) {
                    printf("\n");
                }
                printf("%f ", matrix[i]);
            }
            printf("\n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

double calculateDeterminant(double* matrix, int size)
{
    double a = 4;
    double b = sqrt(a);
    double determinant = 0;
    for (int i = 0; i < size; i++) {
        determinant += matrix[i] * matrix[i];
    }
    return determinant;
}

double calculateNorm(double* matrix, int size)
{
    double determinant = calculateDeterminant(matrix, size);
    return sqrt(determinant);
}

void fillVectorWithValue(double* vector, int size, int value)
{
    for (int i = 0; i < size; i++) {
        vector[i] = value;
    }
}

void vectorSubtraction(double* minuend, double* subtrahend, double* result, int vectorSize)
{
    for (int i = 0; i < vectorSize; i++) {
        result[i] = minuend[i] - subtrahend[i];
    }
}

void multiplyVectorByScalar(double* vector, double* result, int vectorSize, double scalar)
{
    for (int i = 0; i < vectorSize; i++) {
        result[i] = vector[i] * scalar;
    }
}

//vector != result
void multiplyPartMatrixByPartVector(double* partMatrix, int partMatrixSize, double* multiplyVector, int vectorSize,
                                    double* result, int resultSize, int shift, int rank, int nProcesses, int matrixSideSize)
{
    MPI_Status status;
    fillVectorWithValue(result, resultSize, ZERO_VALUE);
    for (int partsDone = 0; partsDone < nProcesses; partsDone++) {
        
        int lineShift = vectorSize * rank;
        for (int i = 0; i < partMatrixSize / matrixSideSize; i++) {
            for (int j = 0; j < vectorSize; j++) {
                result[i] += partMatrix[i * matrixSideSize + lineShift + j] * multiplyVector[j];
            }
        }
        int sendRank = (rank + 1) % nProcesses;
        int recvRank = (rank + nProcesses - 1) % nProcesses;
        if (rank == 0) {
            printf("send:%d, recv:%d, vSize:%d\n", sendRank, recvRank, vectorSize);
            printVector(multiplyVector, vectorSize, rank);
        }
        MPI_Sendrecv_replace(multiplyVector, vectorSize, MPI_DOUBLE, sendRank, 0, recvRank, 0, MPI_COMM_WORLD, &status);
    }
}

double* generateZeroVector(int vectorSize)
{
    double* vector = new double[vectorSize];
    fillVectorWithValue(vector, vectorSize, ZERO_VALUE);
    return vector;
}

double* generateSolutionVector(int vectorSize)
{
    double* solutionVector = generateZeroVector(vectorSize);
    fillVectorWithValue(solutionVector, vectorSize, ZERO_VALUE);
    for (int i = 0; i < vectorSize; i++) {
        solutionVector[i] = i;
    }
    return solutionVector;
}

double* generatePartSolutionVector(int partVectorSize)
{
    double* partSolutionVector = generateZeroVector(partVectorSize);
    fillVectorWithValue(partSolutionVector, partVectorSize, ZERO_VALUE);
    return partSolutionVector;
}

double* generateRightSideVector(int vectorSize)
{
    double* rightSideVector = generateZeroVector(vectorSize);
    fillVectorWithValue(rightSideVector, vectorSize, vectorSize + 1);
    return rightSideVector;
}

double* generateRightPartVector(double* rightSideVector, int rightSideVectorSize, int partVectorSize, int rank)
{
    double* rightPartVector = generateZeroVector(partVectorSize);
    for (int i = 0; i < partVectorSize; i++) {
        int elementIndex = rank * partVectorSize + i;
        if (elementIndex < rightSideVectorSize) {
            rightPartVector[i] = rightSideVector[elementIndex];
        }
        else {
            rightPartVector[i] = 0;
        }
    }
    return rightPartVector;
}

double* generateSquareMatrix(int sideSize)
{
    double* matrix = new double[sideSize * sideSize];
    for (int i = 0; i < sideSize; i++) {
        for (int j = 0; j < sideSize; j++) {
            if (i == j)
                matrix[i * sideSize + j] = 2;
            else
                matrix[i * sideSize + j] = 1;
        }
    }
    return matrix;
}

int calculatePartMatrixSize(int matrixSize, int rank, int nProcesses)
{
    int partMatrixSize = matrixSize / nProcesses;
    if (matrixSize % nProcesses != 0) {
        partMatrixSize++;
    }
    return partMatrixSize;
}

int calculateShift(int matrixSize, int partMatrixSize, int rank, int nProcesses)
{
    int valuesLeft = matrixSize % nProcesses;
    int shift = 0;
    shift = rank * partMatrixSize;
    return shift;
}

double* generatePartMatrix(double* matrix, int matrixSize, int shift, int partMatrixSize)
{
    //double* partMatrix = new double[partMatrixSize];
    double* partMatrix = generateZeroVector(partMatrixSize);
    int additionalSize = 0;
    while (shift + additionalSize < matrixSize && additionalSize < partMatrixSize) {
        additionalSize++;
    }
    memcpy(partMatrix, &matrix[shift], partMatrixSize * sizeof(double));
    return partMatrix;
}

bool isAccuracyAchieved(double numerator, double denominator)
{
    return ((numerator / denominator) < EPSILON);
}

void copyVector(double* source, double* destination, int size)
{
    memcpy(destination, source, size * sizeof(double));
}

int calculatePartVectorSize(int partMatrixSize, int nProcesses)
{
    int partVectorSize = partMatrixSize;
    while (partMatrixSize % nProcesses != 0) {
        partVectorSize++;
    }
    return partVectorSize / nProcesses;
}

//x^(n+1) = x^n – τ(Ax^n – b)
double* iterationMethod(double* matrix, double* rightSideVector, int matrixSideSize, int rank, int nProcesses)
{
    int partMatrixSize = calculatePartMatrixSize(matrixSideSize * matrixSideSize, rank, nProcesses);
    int shift = calculateShift(matrixSideSize * matrixSideSize, partMatrixSize, rank, nProcesses);
    double* partMatrix = generateZeroVector(partMatrixSize);
    MPI_Scatter(matrix, partMatrixSize, MPI_DOUBLE, partMatrix, partMatrixSize, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    int partVectorSize = calculatePartVectorSize(matrixSideSize, nProcesses);
    int partsUnitySize = partVectorSize * nProcesses;
    double* rightPartVector = generateRightPartVector(rightSideVector, matrixSideSize, partVectorSize, rank);
    double* solutionVector = generateSolutionVector(partsUnitySize);
    double* solutionPartVector = generateZeroVector(partVectorSize);
    MPI_Scatter(solutionVector, partVectorSize, MPI_DOUBLE, solutionPartVector, partVectorSize, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    double* scatteredVector = generateZeroVector(partVectorSize);
    double* multiplyResultVector = generateZeroVector(partsUnitySize);
    double* multiplyMatrixVector = generateZeroVector(partsUnitySize);
    double partDeterminant = 0, rightSideNorm = 0, resultNorm = 0;
    partDeterminant = calculateDeterminant(rightPartVector, partVectorSize);
    MPI_Allreduce(&partDeterminant, &rightSideNorm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    rightSideNorm = sqrt(rightSideNorm);
    bool run = true;
    //printVector(solutionPartVector, partVectorSize, rank);
    while (run) {
        //Ax
        multiplyPartMatrixByPartVector(partMatrix, partMatrixSize, solutionPartVector, partVectorSize,
            multiplyResultVector, partsUnitySize, shift, rank, nProcesses, matrixSideSize);
        MPI_Allreduce(multiplyResultVector, multiplyMatrixVector, partsUnitySize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Scatter(multiplyMatrixVector, partVectorSize, MPI_DOUBLE, scatteredVector, partVectorSize,
            MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
        //Ax-b
        vectorSubtraction(scatteredVector, rightSideVector, scatteredVector, partVectorSize);
        partDeterminant = calculateDeterminant(scatteredVector, partVectorSize);
        MPI_Allreduce(&partDeterminant, &resultNorm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        //t(Ax-b)
        multiplyVectorByScalar(scatteredVector, scatteredVector, partVectorSize, TAU);
        //x-t(Ax-b)
        vectorSubtraction(solutionPartVector, scatteredVector, scatteredVector, partVectorSize);
        copyVector(scatteredVector, solutionPartVector, partVectorSize);
        run = !isAccuracyAchieved(sqrt(resultNorm), rightSideNorm);
        run = false;
    }// while (run);
    MPI_Gather(scatteredVector, partVectorSize, MPI_DOUBLE, solutionVector, partVectorSize, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    delete[] partMatrix;
    delete[] rightPartVector;
    delete[] solutionPartVector;
    delete[] scatteredVector;
    delete[] multiplyResultVector;
    delete[] multiplyMatrixVector;
    return solutionVector;
}

int main(int argc, char** argv)
{
    double* matrix = generateSquareMatrix(SIZE_VECTOR);
    double* rightPartVector = generateRightSideVector(SIZE_VECTOR);
    int rank, nProcesses;
    MPI_Init(&argc, &argv);
    double start = MPI_Wtime();
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double* solutionVector = iterationMethod(matrix, rightPartVector, SIZE_VECTOR, rank, nProcesses);
    double finish = MPI_Wtime();
    if (rank == 0) {
        printf("Time: %f\n", finish - start);
    }
    //printVector(solutionVector, SIZE_VECTOR, 0);
    delete[] matrix;
    delete[] solutionVector;
    delete[] rightPartVector;
    MPI_Finalize();
}