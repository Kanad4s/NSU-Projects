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
        printf("%f ", vector[i]);
    }
    printf("\n");
}

void printPartMatrix(double* matrix, int partSize, int shift, int matrixSideSize,
    int nProcesses, int rank)
{
    for (int i = 0; i < nProcesses; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (i == rank) {
            printf("\n");
            //free space
            for (int i = 0; i < shift % matrixSideSize; i++) {
                printf("  ");
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
void multiplyPartMatrixByPartVector(double* partMatrix, int partMatrixSize, double* multiplyVector,
                                    double* resultVector, int vectorSize, int rank, int nProcesses)
{
    fillVectorWithValue(resultVector, vectorSize, ZERO_VALUE);
    MPI_Status status;
    int recvRank = (rank + 1) % nProcesses;
    int sendRank = (rank + nProcesses - 1) % nProcesses;
    int columnShift = rank * vectorSize;
    int lineSize = partMatrixSize / vectorSize;
    for (int partsDone = 0; partsDone < nProcesses; partsDone++) {
        int lineShift = ((rank + partsDone) % nProcesses) * vectorSize;
        for (int i = 0; i < vectorSize; i++) {
            for (int j = 0; j < vectorSize; j++) {
                resultVector[i] += partMatrix[i * lineSize + lineShift + j] * multiplyVector[j];
            }
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
    /*for (int i = 0; i < vectorSize; i++) {
        solutionVector[i] = i;
    }*/
    return solutionVector;
}

double* generateRightSideVector(int vectorSize)
{
    double* rightSideVector = generateZeroVector(vectorSize);
    fillVectorWithValue(rightSideVector, vectorSize, vectorSize + 1);
    return rightSideVector;
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
    return partMatrixSize / nProcesses;
}

//x^(n+1) = x^n – τ(Ax^n – b)
double* iterationMethod(double* matrix, double* rightSideVector, int matrixSideSize, int rank, int nProcesses)
{
    int partMatrixSize = calculatePartMatrixSize(matrixSideSize * matrixSideSize, rank, nProcesses);
    double* partMatrix = generateZeroVector(partMatrixSize);
    MPI_Scatter(matrix, partMatrixSize, MPI_DOUBLE, partMatrix, partMatrixSize, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    int partVectorSize = calculatePartVectorSize(matrixSideSize, nProcesses);
    double* rightPartVector = generateZeroVector(partVectorSize);
    MPI_Scatter(rightSideVector, partVectorSize, MPI_DOUBLE, rightPartVector, partVectorSize, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    double* solutionVector = generateSolutionVector(matrixSideSize);
    double* solutionPartVector = generateZeroVector(partVectorSize);
    MPI_Scatter(solutionVector, partVectorSize, MPI_DOUBLE, solutionPartVector, partVectorSize, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    double* scatteredVector = generateZeroVector(partVectorSize);
    double* multiplyResultVector = generateZeroVector(partVectorSize);
    double partDeterminant = 0, rightSideNorm = 0, resultNorm = 0;
    partDeterminant = calculateDeterminant(rightPartVector, partVectorSize);
    MPI_Allreduce(&partDeterminant, &rightSideNorm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    rightSideNorm = sqrt(rightSideNorm);
    bool run = true;
    while (run) {
        //Ax
        multiplyPartMatrixByPartVector(partMatrix, partMatrixSize, solutionPartVector, multiplyResultVector, 
                                       partVectorSize, rank, nProcesses);
        //Ax-b
        vectorSubtraction(multiplyResultVector, rightPartVector, multiplyResultVector, partVectorSize);
        partDeterminant = calculateDeterminant(multiplyResultVector, partVectorSize);
        MPI_Allreduce(&partDeterminant, &resultNorm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        resultNorm = sqrt(resultNorm);
        //t(Ax-b)
        multiplyVectorByScalar(multiplyResultVector, multiplyResultVector, partVectorSize, TAU);
        //x-t(Ax-b)
        vectorSubtraction(solutionPartVector, multiplyResultVector, multiplyResultVector, partVectorSize);
        copyVector(multiplyResultVector, solutionPartVector, partVectorSize);
        run = !isAccuracyAchieved(resultNorm, rightSideNorm);
        //run = false;
    }
    MPI_Gather(solutionPartVector, partVectorSize, MPI_DOUBLE, solutionVector, partVectorSize, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    delete[] partMatrix;
    delete[] rightPartVector;
    delete[] solutionPartVector;
    delete[] scatteredVector;
    delete[] multiplyResultVector;
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
        printVector(solutionVector, SIZE_VECTOR, 0);
    }
    delete[] matrix;
    delete[] solutionVector;
    delete[] rightPartVector;
    MPI_Finalize();
}