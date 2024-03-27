#include "mpi.h"
#include <cstring>
#include <iostream>
#include <math.h>
#include <omp.h>

enum {
    SIZE_VECTOR = 8,//4096,
    ZERO_VALUE = 0
};

const double TAU = 1e-5;
const double EPSILON = 1e-5;

void printVector(const double* vector, const int size)
{
    for (int i = 0; i < size; i++) {
        std::cout << vector[i] << " ";
    }
    std::cout << std::endl;
}

void printPartMatrix(const double* matrix, const int partSize, const int shift, const int matrixSize, const int nProcesses, const int rank)
{
    for (int i = 0; i < nProcesses; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (i == rank) {
            //printf("rank = %d\n", rank);
            //printVector(partMatrix, partMatrixSize);
            //free space
            for (int i = 0; i < shift % matrixSize; i++) {
                std::cout << "  ";
            }
            //part of matrix
            for (int i = 0; i < partSize; i++) {
                if (i != 0 && (i + shift) % matrixSize == 0) {
                    std::cout << std::endl;
                }
                std::cout << matrix[i] << " ";
            }
            std::cout << std::endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

double calculateDeterminant(const double* matrix, const int size)
{
    double determinant = 0;
    for (int i = 0; i < size; i++) {
        determinant += matrix[i] * matrix[i];
    }
    return sqrt(determinant);
}

void fillVectorWithValue(double* vector, const int size, const int value)
{
    for (int i = 0; i < size; i++) {
        vector[i] = value;
    }
}

void vectorSubtraction(const double* minuend, const double* subtrahend, double* result, const int size)
{
    for (int i = 0; i < size; i++) {
        result[i] = minuend[i] - subtrahend[i];
    }
}

void multiplyVectorByScalar(double* vector, double* result, const int size, const double scalar)
{
    for (int i = 0; i < size; i++) {
        result[i] = vector[i] * scalar;
    }
}

void multiplyMatrixByVector(const double* matrix, const double* vector, double* result, const int size)
{
    fillVectorWithValue(result, size, ZERO_VALUE);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i] += matrix[i * size + j] * vector[j];
        }
    }
}

double* generateZeroVector(const int size)
{
    double* vector = new double[size];
    fillVectorWithValue(vector, size, ZERO_VALUE);
    return vector;
}

double* generateSolutionVector(const int size)
{
    double* solutionVector = generateZeroVector(size);
    fillVectorWithValue(solutionVector, size, ZERO_VALUE);
    return solutionVector;
}

double* generateRightPartVector(const int size)
{
    double* rightPartVector = generateZeroVector(size);
    fillVectorWithValue(rightPartVector, size, size + 1);
    return rightPartVector;
}

double* generateSquareMatrix(const int size)
{
    double* matrix = new double[size * size];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j)
                matrix[i * size + j] = 2;
            else
                matrix[i * size + j] = 1;
        }
    }
    return matrix;
}

int calculatePartMatrixSize(const int matrixSize, const int rank, const int nProcesses)
{
    int partMatrixSize = matrixSize / nProcesses;
    int valuesLeft = matrixSize % nProcesses;
    if (valuesLeft != 0 && rank < valuesLeft) {
        partMatrixSize++;
    }
    //std::cout << "rank = " << rank << ", newSize = " << newSize << std::endl;
    return partMatrixSize;
}

int calculateShift(const int matrixSize, const int partMatrixSize, const int rank, const int nProcesses)
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

double* generatePartMatrix(const double* matrix, const int matrixSize, const int rank, const int nProcesses)
{
    int partMatrixSize = calculatePartMatrixSize(matrixSize * matrixSize, rank, nProcesses);
    int shift = calculateShift(matrixSize * matrixSize, partMatrixSize, rank, nProcesses);
    double* partMatrix = new double[partMatrixSize];
    memcpy(partMatrix, &matrix[shift], partMatrixSize * sizeof(double));
    printPartMatrix(partMatrix, partMatrixSize, shift, matrixSize, nProcesses, rank);
    return partMatrix;
}

bool isAccuracyAchieved(const double numerator, const double denominator)
{
    return ((numerator / denominator) < EPSILON);
}

void copyVector(const double* vector, double* copy, const int size)
{
    memcpy(copy, vector, size * sizeof(double));
}

double* iterationMethod(const double* matrix, const double* rightPartVector, const int matrixSize, const int rank, const int nProcesses)
{
    double* solutionVector = generateSolutionVector(matrixSize);
    double* solutionCopyVector = generateZeroVector(matrixSize);
    double* multiplyVector = generateZeroVector(matrixSize);
    double rightPartDeterminant = calculateDeterminant(rightPartVector, matrixSize);
    bool run = true;
    do {
        copyVector(solutionVector, solutionCopyVector, matrixSize);
        multiplyMatrixByVector(matrix, solutionCopyVector, multiplyVector, matrixSize);
        vectorSubtraction(multiplyVector, rightPartVector, multiplyVector, matrixSize);
        multiplyVectorByScalar(multiplyVector, solutionCopyVector, matrixSize, TAU);
        vectorSubtraction(solutionVector, solutionCopyVector, solutionVector, matrixSize);
        run = !isAccuracyAchieved(calculateDeterminant(multiplyVector, matrixSize), rightPartDeterminant);
    } while (run);
    delete[] solutionCopyVector;
    delete[] multiplyVector;
    return solutionVector;
}

int main(int argc, char** argv)
{
    double* matrix = generateSquareMatrix(SIZE_VECTOR);
    double* rightPartVector = generateRightPartVector(SIZE_VECTOR);
    int rank, nProcesses;
    MPI_Init(&argc, &argv);
    double start = MPI_Wtime();
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double* testMatrix = generatePartMatrix(matrix, SIZE_VECTOR, rank, nProcesses);
    //double* solutionVector = iterationMethod(matrix, rightPartVector, SIZE_VECTOR, rank, nProcesses);
    double finish = MPI_Wtime();
    std::cout << "TIME: " << finish - start << std::endl;
    delete[] matrix;
    //delete[] solutionVector;
    delete[] rightPartVector;
    //printf("Hello World! proc:%d\n", rank);
    MPI_Finalize();
    //std::cout << "process: " << rank << " size: " << nProcesses << std::endl;
    //std::cout << "End\n";
}