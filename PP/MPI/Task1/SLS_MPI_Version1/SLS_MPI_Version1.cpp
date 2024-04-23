#include "mpi.h"
#include <cstring>
#include <iostream>
#include <math.h>
#include <omp.h>

enum {
    SIZE_VECTOR = 1024,//4096,
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
            //printf("rank = %d\n", rank);
            //printVector(partMatrix, partMatrixSize);
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
                printf("%1.0f ", matrix[i]);
            }
            printf("\n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

double calculateDeterminant(double* matrix, int size)
{
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

void vectorPartSubtraction(double* minuend, double* subtrahend, double* result, int vectorSize, int rank, int nProcesses, int partSize)
{
    int shift = rank * partSize;
    if (rank + 1 == nProcesses && partSize != 0) {
        partSize = vectorSize - shift;
    }
    for (int i = 0; i < partSize; i++) {
        result[shift + i] = minuend[shift + i] - subtrahend[shift + i];
    }
}

void multiplyVectorPartByScalar(double* vector, double* result, int vectorSize, double scalar, int rank, int nProcesses, int partSize)
{
    int shift = rank * partSize;
    if (rank + 1 == nProcesses && partSize != 0) {
        partSize = vectorSize - shift;
    }
    for (int i = 0; i < partSize; i++) {
        result[shift + i] = vector[shift + i] * scalar;
    }
}

//vector != result
void multiplyPartMatrixByVector(double* partMatrix, int partMatrixSize, double* multiplyVector, double* result, int vectorSize,
                                 int shift)
{
    fillVectorWithValue(result, vectorSize, ZERO_VALUE);
    int multiplyVectorLineIndex = shift % vectorSize;
    int resultLineIndex = shift / vectorSize;
    for (int i = 0; i < partMatrixSize; i++) {
        result[resultLineIndex] += partMatrix[i] * multiplyVector[multiplyVectorLineIndex];
        ++multiplyVectorLineIndex;
        if (multiplyVectorLineIndex % vectorSize == 0) {
            multiplyVectorLineIndex = 0;
            resultLineIndex++;
        }
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
    int valuesLeft = matrixSize % nProcesses;
    if (valuesLeft != 0 && rank < valuesLeft) {
        partMatrixSize++;
    }
    return partMatrixSize;
}

int calculateMatrixShift(int matrixSize, int partMatrixSize, int rank, int nProcesses)
{
    int valuesLeft = matrixSize % nProcesses;
    int shift = 0;
    if (rank < valuesLeft) {
        shift = rank * partMatrixSize;
    }
    else {
        shift = rank * partMatrixSize + valuesLeft;
    }
    return shift;
}

int calculateVectorProcessingPart(int vectorSize, int rank, int nProcesses)
{
    int processPart;
    if (vectorSize < nProcesses) {
        rank < vectorSize ? processPart = 1 : processPart = 0;
    }
    else {
        processPart = vectorSize / nProcesses;
    }
    return processPart;
}

double* generatePartMatrix(double* matrix, int matrixSize, int shift, int partMatrixSize)
{
    double* partMatrix = generateZeroVector(partMatrixSize);
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

void prepareForReduce(double* vector, int vectorSize, int matrixShift, int partMatrixSize, int rank)
{
    int firstPart = matrixShift / vectorSize;
    for (int i = 0; i < firstPart; i++) {
        vector[i] = 0;
    }
    int lastPartBegin = (matrixShift + partMatrixSize) / vectorSize;
    if ((matrixShift + partMatrixSize) % vectorSize != 0) {
        lastPartBegin++;
    }
    for (int i = lastPartBegin; i < vectorSize; i++) {
        vector[i] = 0;
    }
}

//x^(n+1) = x^n – τ(Ax^n – b)
double* iterationMethod(double* matrix, double* rightSideVector, int matrixSideSize, int rank, int nProcesses)
{
    int partMatrixSize = calculatePartMatrixSize(matrixSideSize * matrixSideSize, rank, nProcesses);
    int matrixShift = calculateMatrixShift(matrixSideSize * matrixSideSize, partMatrixSize, rank, nProcesses);
    double* partMatrix = generatePartMatrix(matrix, matrixSideSize * matrixSideSize, matrixShift, partMatrixSize);
    int vectorProcessingPart = calculateVectorProcessingPart(matrixSideSize, rank, nProcesses);
    double* solutionVector = generateSolutionVector(matrixSideSize);
    double* solutionReduceVector = generateZeroVector(matrixSideSize);
    double* processingVector = generateZeroVector(matrixSideSize);
    double* normVector = generateZeroVector(matrixSideSize);
    double solutionNorm = 0;
    double rightSideNorm = calculateNorm(rightSideVector, matrixSideSize);
    bool run = true;
    while (run) {
        //Ax
        multiplyPartMatrixByVector(partMatrix, partMatrixSize, solutionVector,
            processingVector, matrixSideSize, matrixShift);
        //Ax-b
        vectorPartSubtraction(processingVector, rightSideVector, processingVector, matrixSideSize, rank, nProcesses, vectorProcessingPart);
        prepareForReduce(processingVector, matrixSideSize, matrixShift, partMatrixSize, rank);
        MPI_Allreduce(processingVector, normVector, matrixSideSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        solutionNorm = calculateNorm(normVector, matrixSideSize);
        //t(Ax-b)
        multiplyVectorPartByScalar(processingVector, processingVector, matrixSideSize, TAU, rank, nProcesses, vectorProcessingPart);
        //x-t(Ax-b)
        vectorPartSubtraction(solutionVector, processingVector, solutionReduceVector, matrixSideSize, rank, nProcesses, vectorProcessingPart);
        prepareForReduce(solutionReduceVector, matrixSideSize, matrixShift, partMatrixSize, rank);
        MPI_Allreduce(solutionReduceVector, solutionVector, matrixSideSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        run = !isAccuracyAchieved(solutionNorm, rightSideNorm);
    }
    delete[] partMatrix;
    delete[] solutionReduceVector;
    delete[] processingVector;
    delete[] normVector;
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
        //printVector(solutionVector, SIZE_VECTOR, 0);
    }
    delete[] matrix;
    delete[] solutionVector;
    delete[] rightPartVector;
    MPI_Finalize();
}