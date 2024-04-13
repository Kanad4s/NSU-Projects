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

void printPartMatrix(const double* matrix, const int partSize, const int shift, const int matrixSize,
                     const int nProcesses, const int rank)
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

//todo
void multiplyPartMatrixByVector(const double* partMatrix, const double* vector, double* result, const int vectorSize,
                                const int partMatrixSize, const int shift, const int rank, const int nProcesses)
{
    fillVectorWithValue(result, vectorSize, ZERO_VALUE);
    int lineShift = shift % vectorSize;
    int vectorIndex = lineShift;
    for (int i = 0; i < partMatrixSize; i++) {
        result[i] += partMatrix[i] * vector[vectorIndex];
        vectorIndex++;
        if (vectorIndex % vectorSize == 0) {
            vectorIndex = 0;
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

double* generatePartMatrix(const double* matrix, const int matrixSize, const int shift, const int partMatrixSize)
{
    double* partMatrix = new double[partMatrixSize];
    memcpy(partMatrix, &matrix[shift], partMatrixSize * sizeof(double));
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

//x^(n+1) = x^n – τ(Ax^n – b)
double* iterationMethod(const double* matrix, const double* rightPartVector, const int matrixSideSize, const int rank, const int nProcesses)
{
    int partMatrixSize = calculatePartMatrixSize(matrixSideSize * matrixSideSize, rank, nProcesses);
    int shift = calculateShift(matrixSideSize * matrixSideSize, partMatrixSize, rank, nProcesses);
    double* partMatrix = generatePartMatrix(matrix, matrixSideSize, shift, partMatrixSize);
    //printPartMatrix(partMatrix, partMatrixSize, shift, matrixSize, nProcesses, rank);

    double* solutionVector = generateSolutionVector(matrixSideSize);
    double* solutionCopyVector = generateZeroVector(matrixSideSize);
    double* multiplyVector = generateZeroVector(matrixSideSize);
    double rightPartDeterminant = calculateDeterminant(rightPartVector, matrixSideSize);
    bool run = true;
    do {
        copyVector(solutionVector, solutionCopyVector, matrixSideSize);
        //todo
        multiplyPartMatrixByVector(matrix, solutionCopyVector, multiplyVector, matrixSideSize);
        vectorSubtraction(multiplyVector, rightPartVector, multiplyVector, matrixSideSize);
        multiplyVectorByScalar(multiplyVector, solutionCopyVector, matrixSideSize, TAU);
        vectorSubtraction(solutionVector, solutionCopyVector, solutionVector, matrixSideSize);
        run = !isAccuracyAchieved(calculateDeterminant(multiplyVector, matrixSideSize), rightPartDeterminant);
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