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

void printVector(double* vector, int size, int rank)
{
    printf("rank:%d\n", rank);
    for (int i = 0; i < size; i++) {
        std::cout << vector[i] << " ";
    }
    std::cout << std::endl;
}

void printPartMatrix(double* matrix, int partSize, int shift, int matrixSize,
                     int nProcesses, int rank)
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

double calculateDeterminant(double* matrix, int size)
{
    double determinant = 0;
    for (int i = 0; i < size; i++) {
        determinant += matrix[i] * matrix[i];
    }
    return sqrt(determinant);
}

void fillVectorWithValue(double* vector, int size, int value)
{
    for (int i = 0; i < size; i++) {
        vector[i] = value;
    }
}

void vectorSubtraction( double* minuend, double* subtrahend, double* result, int size)
{
    for (int i = 0; i < size; i++) {
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
void multiplyPartMatrixByVector(double* partMatrix, double* multiplyVector, double* result, int vectorSize,
                                int partMatrixSize, int shift)
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

double* generateRightPartVector(int vectorSize)
{
    double* rightPartVector = generateZeroVector(vectorSize);
    fillVectorWithValue(rightPartVector, vectorSize, vectorSize + 1);
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
    int valuesLeft = matrixSize % nProcesses;
    if (valuesLeft != 0 && rank < valuesLeft) {
        partMatrixSize++;
    }
    //std::cout << "rank = " << rank << ", newSize = " << newSize << std::endl;
    return partMatrixSize;
}

int calculateShift(int matrixSize, int partMatrixSize, int rank, int nProcesses)
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

double* generatePartMatrix(double* matrix, int matrixSize, int shift, int partMatrixSize)
{
    double* partMatrix = new double[partMatrixSize];
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

//x^(n+1) = x^n – τ(Ax^n – b)
double* iterationMethod(double* matrix, double* rightPartVector, int matrixSideSize, int rank, int nProcesses)
{
    int partMatrixSize = calculatePartMatrixSize(matrixSideSize * matrixSideSize, rank, nProcesses);
    int shift = calculateShift(matrixSideSize * matrixSideSize, partMatrixSize, rank, nProcesses);
    double* partMatrix = generatePartMatrix(matrix, matrixSideSize, shift, partMatrixSize);
    printPartMatrix(partMatrix, partMatrixSize, shift, matrixSideSize, nProcesses, rank);

    double* solutionVector = generateSolutionVector(matrixSideSize);
    double* solutionCopyVector = generateZeroVector(matrixSideSize);
    double* multiplyResultVector = generateZeroVector(matrixSideSize);
    double rightPartDeterminant = calculateDeterminant(rightPartVector, matrixSideSize);
    bool run = true;
    do {
        copyVector(solutionVector, solutionCopyVector, matrixSideSize);
        multiplyPartMatrixByVector(matrix, solutionVector, multiplyResultVector, matrixSideSize, partMatrixSize, shift);
        printVector(multiplyResultVector, matrixSideSize, rank);
        //need buffer to allreduce
        MPI_Allreduce(multiplyResultVector, multiplyResultVector, partMatrixSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        printVector(multiplyResultVector, matrixSideSize, rank);
        //todo
        vectorSubtraction(multiplyResultVector, rightPartVector, multiplyResultVector, matrixSideSize);
        /*multiplyVectorByScalar(multiplyResultVector, solutionCopyVector, matrixSideSize, TAU);
        vectorSubtraction(solutionVector, solutionCopyVector, solutionVector, matrixSideSize);
        run = !isAccuracyAchieved(calculateDeterminant(multiplyResultVector, matrixSideSize), rightPartDeterminant);*/
        run = false;
    } while (run);
    delete[] solutionCopyVector;
    delete[] multiplyResultVector;
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
    //double* testMatrix = generatePartMatrix(matrix, SIZE_VECTOR, rank, nProcesses);
    double* solutionVector = iterationMethod(matrix, rightPartVector, SIZE_VECTOR, rank, nProcesses);
    double finish = MPI_Wtime();
    std::cout << "TIME: " << finish - start << std::endl;
    delete[] matrix;
    delete[] solutionVector;
    delete[] rightPartVector;
    //printf("Hello World! proc:%d\n", rank);
    MPI_Finalize();
    //std::cout << "process: " << rank << " size: " << nProcesses << std::endl;
    //std::cout << "End\n";
    //mpiexec -n 4 .\SLS_MPI_Version1.exe
}