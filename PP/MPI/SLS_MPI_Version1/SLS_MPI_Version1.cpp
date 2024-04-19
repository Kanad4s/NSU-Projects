#include "mpi.h"
#include <cstring>
#include <iostream>
#include <math.h>
#include <omp.h>

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

void multiplyPartMatrixByPartVector(double* partMatrix, int partMatrixSize, double* multiplyVector, int partVectorSize, 
                                    double* result, int resultSize, int shift, int rank)
{
    fillVectorWithValue(result, resultSize, ZERO_VALUE);
    int resultLineIndex = shift % SIZE_VECTOR;
    for (int i = 0; i < partVectorSize; i++) {
        for (int j = 0; j < resultSize; j++) {
            int matrixElementLineIndex = i * resultSize + j;
            result[j] += partMatrix[matrixElementLineIndex] * multiplyVector[i];
            if (matrixElementLineIndex % SIZE_VECTOR == 0) {
                resultLineIndex++;
            }
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
    /*int valuesLeft = matrixSize % nProcesses;
    if (valuesLeft != 0 && rank < valuesLeft) {
        partMatrixSize++;
    }*/
    if (matrixSize % nProcesses != 0) {
        partMatrixSize++;
    }
    //std::cout << "rank = " << rank << ", newSize = " << newSize << std::endl;
    return partMatrixSize;
}

int calculateShift(int matrixSize, int partMatrixSize, int rank, int nProcesses)
{
    int valuesLeft = matrixSize % nProcesses;
    int shift = 0;
    /*if (rank < valuesLeft) {
        shift = rank * partMatrixSize;
    }
    else {
        shift = rank * partMatrixSize + valuesLeft;
    }*/
    shift = rank * partMatrixSize;
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

int calculatePartVectorSize(int partMatrixSize, int nProcesses) 
{
    int partVectorSize = partMatrixSize;
    while (partMatrixSize % nProcesses != 0) {
        partVectorSize++;
    }
    return partVectorSize / nProcesses;
}

int GetCntCurrentFillLineMatrix(const int rank, const int cntProcess, const int newSize) {
    int resultFillLine = 0;
    for (int startRank = 1; startRank <= rank; ++startRank) {
        resultFillLine += newSize / cntProcess;
    }
    return resultFillLine;
}

double* generatePartMatrix1(const int rank, const int cntProcess, const int fictitiousSize) {
    int partFictitiousSizeMatrix = fictitiousSize * (fictitiousSize / cntProcess);
    printf("%d\n", partFictitiousSizeMatrix);
    double* partMatrix = generateZeroVector(partFictitiousSizeMatrix);

    int countRows = fictitiousSize / cntProcess;
    int index = countRows * rank;
    int numberCntLine = GetCntCurrentFillLineMatrix(rank, cntProcess, fictitiousSize);

    for (int i = 0, offset = 0; i < countRows; ++i, offset += fictitiousSize) {
        if (numberCntLine + i + 1 <= SIZE_VECTOR) {
            for (int j = 0; j < SIZE_VECTOR; ++j) {
                partMatrix[offset + j] = 1.0;
            }
            partMatrix[index + i] = 2.0;
        }
        index += fictitiousSize;
    }
    return partMatrix;
}

//x^(n+1) = x^n – τ(Ax^n – b)
double* iterationMethod(double* matrix, double* rightSideVector, int matrixSideSize, int rank, int nProcesses)
{
    int partMatrixSize = calculatePartMatrixSize(matrixSideSize * matrixSideSize, rank, nProcesses);
    int shift = calculateShift(matrixSideSize * matrixSideSize, partMatrixSize, rank, nProcesses);
    //double* partMatrix = generatePartMatrix(matrix, matrixSideSize, shift, partMatrixSize);
    //printPartMatrix(partMatrix, partMatrixSize, shift, matrixSideSize, nProcesses, rank);
    int partVectorSize = calculatePartVectorSize(matrixSideSize, nProcesses);
    int partsUnitySize = partVectorSize * nProcesses;
    double* partMatrix = generatePartMatrix1(rank, nProcesses, partVectorSize * nProcesses);
    //printf("%d, sizeVector: %d, sizeUnity: %d\n", rank, partVectorSize, partsUnitySize);
    double* rightPartVector = generateRightPartVector(rightSideVector, matrixSideSize, partVectorSize, rank);
    double* solutionVector = generateSolutionVector(partsUnitySize);
    double* solutionPartVector = generateSolutionVector(partVectorSize);
    double* scatteredVector = generateZeroVector(partVectorSize);
    double* multiplyResultVector = generateZeroVector(partsUnitySize);
    double* multiplyMatrixVector = generateZeroVector(partsUnitySize);
    double partDeterminant = 0, rightSideNorm = 0, resultNorm = 0;
    partDeterminant = calculateDeterminant(rightPartVector, partVectorSize);
    MPI_Allreduce(&partDeterminant, &rightSideNorm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    rightSideNorm = sqrt(rightSideNorm);
    //printf("rank: %d, %f,%f\n", rank, partDeterminant, rightSideNorm);
    bool run = true;
    int i = 0;
    do {
        //Ax
        multiplyPartMatrixByPartVector(partMatrix, partMatrixSize, solutionPartVector, partVectorSize,
                                       multiplyResultVector, partsUnitySize, shift, rank);
        //printVector(multiplyResultVector, partsUnitySize, rank);
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
        //printVector(solutionVector, matrixSideSize, rank);
        copyVector(scatteredVector, solutionPartVector, partVectorSize);
        run = !isAccuracyAchieved(sqrt(resultNorm), rightSideNorm);
        i++;
        if (i == 2) {
            run = false;
        }
    } while (run);
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
    //std::cout << "TIME: " << finish - start << std::endl;
    if (rank == 0) {
        printf("Time: %f\n", finish - start);
    }
    //printVector(solutionVector, SIZE_VECTOR, 0);
    delete[] matrix;
    delete[] solutionVector;
    delete[] rightPartVector;
    MPI_Finalize();
    //mpiexec -n 4 .\SLS_MPI_Version1.exe
}