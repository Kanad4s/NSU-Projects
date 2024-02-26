#include <iostream>
#include <math.h>
#include <cstring>
#include <omp.h>

enum {
    SIZE_VECTOR = 4096,
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

double calculateDeterminant(const double* matrix, const int size)
{
    double determinant = 0;
#pragma omp parallel for reduction(+:determinant)
    for (int i = 0; i < size; i++) {
        determinant += matrix[i] * matrix[i];
    }
    return sqrt(determinant);
}

void fillVectorWithValue(double* vector, const int size, const int value)
{
#pragma omp for
    for (int i = 0; i < size; i++) {
        vector[i] = value;
    }
}

void vectorSubtraction(const double* minuend, const double* subtrahend, double* result, const int size)
{
#pragma omp for
    for (int i = 0; i < size; i++) {
        result[i] = minuend[i] - subtrahend[i];
    }
}

void multiplyVectorByScalar(double* vector, double* result, const int size, const double scalar)
{
#pragma omp for
    for (int i = 0; i < size; i++) {
        result[i] = vector[i] * scalar;
    }
}

void multiplyMatrixByVector(const double* matrix, const double* vector, double* result, const int size)
{
    fillVectorWithValue(result, size, ZERO_VALUE);
#pragma omp for
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

bool isAccuracyAchieved(const double numerator, const double denominator)
{
    return ((numerator / denominator) < EPSILON);
}

void copyVector(const double* vector, double* copy, const int size)
{
    memcpy(copy, vector, size * sizeof(double));
}

double* iterationMethod(const double* matrix, double* rightPartVector, const int size)
{
    double* solutionVector = generateSolutionVector(size);
    double* solutionCopyVector = generateZeroVector(size);
    double* multiplyVector = generateZeroVector(size);
    double rightPartDeterminant = calculateDeterminant(rightPartVector, size);
    bool run = true;
#pragma omp parallel
    {
        do {
            copyVector(solutionVector, solutionCopyVector, size);
            multiplyMatrixByVector(matrix, solutionCopyVector, multiplyVector, size);
            vectorSubtraction(multiplyVector, rightPartVector, multiplyVector, size);
            multiplyVectorByScalar(multiplyVector, solutionCopyVector, size, TAU);
            vectorSubtraction(solutionVector, solutionCopyVector, solutionVector, size);
#pragma omp single
            run = !isAccuracyAchieved(calculateDeterminant(multiplyVector, size), rightPartDeterminant);
        } while (run);
    }
    delete[] solutionCopyVector;
    delete[] multiplyVector;
    return solutionVector;
}

int main()
{
    double* matrix = generateSquareMatrix(SIZE_VECTOR);
    double* rightPartVector = generateRightPartVector(SIZE_VECTOR);
    double start = omp_get_wtime();
    double* solutionVector = iterationMethod(matrix, rightPartVector, SIZE_VECTOR);
    double finish = omp_get_wtime();
    std::cout << "TIME: " << finish - start << std::endl;
    //printVector(solutionVector, SIZE_VECTOR);
    delete[] matrix;
    delete[] solutionVector;
    delete[] rightPartVector;
}