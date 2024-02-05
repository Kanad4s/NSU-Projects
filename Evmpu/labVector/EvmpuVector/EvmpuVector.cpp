#include <iostream>
#include <openblas/cblas.h>
#include <ctime>
#include <immintrin.h>

void printMatrix(float matrix[], int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			std::cout << matrix[i * size + j] << " ";
		}
		std::cout << std::endl;
	}
}

float* getOneMatrix(int size) {
	float* matrix = new float[size * size] {};
	for (int i = 0; i < size; i++) {
		matrix[i * size + i] = 1;
	}
	return matrix;
}

float* multMatrix(float a[], float b[], int size)
{
	float* result = new float[size * size] {};
	for (int i = 0; i < size; i++) {
		for (int k = 0; k < size; k++) {
			for (int j = 0; j < size; j++) {
				result[i * size + j] += a[i * size + k] * b[k * size + j];
			}
		}
	}
	return result;
}

float findMaxLine(float matrix[], int size) {
	float currentSum = 0;
	float maxLine = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			currentSum += matrix[i * size + j];
		}
		if (currentSum > maxLine) {
			maxLine = currentSum;
		}
		currentSum = 0;
	}
	return maxLine;
}

float findMaxColumn(float matrix[], int size) {
	float currentSum = 0;
	float maxColumn = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			currentSum += matrix[j * size + i];
		}
		if (currentSum > maxColumn) {
			maxColumn = currentSum;
		}
		currentSum = 0;
	}
	return maxColumn;
}

float* getMatrixB(float matrix[], int size) {
	float divider = findMaxLine(matrix, size) * findMaxColumn(matrix, size);
	float* matrixB = new float[size * size] {};
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			matrixB[j * size + i] = matrix[i * size + j] / divider;
		}
	}
	return matrixB;
}

float* getMatrixR(float matrixA[], float matrixB[], int size) {
	float* matrixR = getOneMatrix(size);
	float* matrixMult = multMatrix(matrixB, matrixA, size);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			matrixR[i * size + j] -= matrixMult[i * size + j];
		}
	}
	return matrixR;
}

float* inverseMatrix(float matrix[], int size, int nMembers) {
	float* matrixB = getMatrixB(matrix, size);
	float* matrixResult = getOneMatrix(size);
	float* matrixR = getMatrixR(matrix, matrixB, size);
	float* matrixDegreeR = matrixR;
	for (int k = 0; k < nMembers; k++) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				matrixResult[i * size + j] += matrixDegreeR[i * size + j];
			}
		}
		if (k != nMembers - 1) {
			matrixDegreeR = multMatrix(matrixDegreeR, matrixR, size);
		}
	}
	matrixResult = multMatrix(matrixResult, matrixB, size);
	delete[] matrixB;
	delete[] matrixR;
	delete[] matrixDegreeR;
	return matrixResult;
}

void multMatrixBlas(float a[], float b[], float result[], int size) {
	float* resultTmp = new float[size * size] {};
	cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, size, size, size, 1.0, a, size, b, size, 0.0, resultTmp, size);
	cblas_scopy(size * size, resultTmp, 1, result, 1);
	delete[] resultTmp;
}

float* inverseMatrixBlas(float matrix[], int size, int nSteps) {
	float* matrixB = getMatrixB(matrix, size);
	float* matrixR = getOneMatrix(size);
	float* matrixResult = getOneMatrix(size);
	cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, size, size, size, -1.0, matrixB, size, matrix, size, 1.0, matrixR, size);
	cblas_saxpy(size * size, 1.0, matrixR, 1, matrixResult, 1);
	float* matrixDegreeR = new float[size * size] {};
	cblas_scopy(size * size, matrixR, 1, matrixDegreeR, 1);
	for (int i = 0; i < nSteps; i++) {
		multMatrixBlas(matrixDegreeR, matrixR, matrixDegreeR, size);
		cblas_saxpy(size * size, 1.0, matrixDegreeR, 1, matrixResult, 1);
	}
	multMatrixBlas(matrixResult, matrixB, matrixResult, size);
	delete[] matrixB;
	delete[] matrixR;
	delete[] matrixDegreeR;
	return matrixResult;
}

float horizontalSum256(__m256 x) {
	__m256 tmp = _mm256_setzero_ps();
	tmp = _mm256_permute2f128_ps(x, x, 1);
	x = _mm256_hadd_ps(x, tmp);
	x = _mm256_hadd_ps(x, x);
	x = _mm256_hadd_ps(x, x);
	float sum = x.m256_f32[0];
	return sum;
}

float findHorizontalMax256(__m256 x) {
	float maxNumber = 0;
	for (int i = 0; i < 8; i++) {
		float curNumber = x.m256_f32[i];
		if (curNumber > maxNumber) {
			maxNumber = curNumber;
		}
	}
	return maxNumber;
}

float findMaxColumnMy(float matrix[], int size) {
	__m256* matrixVector = (__m256*)matrix;
	__m256 currentSum = _mm256_setzero_ps();
	float verticalSum = 0;
	float maxColumn = 0;
	for (int i = 0; i < size / 8; i++) {
		for (int j = 0; j < size ; j++) {
			currentSum = _mm256_add_ps(matrixVector[j * size / 8 + i], currentSum);
		}
		verticalSum = findHorizontalMax256(currentSum);
		if (verticalSum > maxColumn) {
			maxColumn = verticalSum;
		}
		currentSum = _mm256_setzero_ps();
	}
	return maxColumn;
}


float findMaxLineMy(float matrix[], int size) {
	
	__m256* matrixVector = (__m256*)matrix;
	__m256 currentSum = _mm256_setzero_ps();
	float horizontalSum = 0;
	float maxLine = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size / 8; j++) {
			currentSum = _mm256_add_ps(matrixVector[i * size / 8 + j], currentSum);
		}
		horizontalSum = horizontalSum256(currentSum);
		if (maxLine < horizontalSum) {
			maxLine = horizontalSum;
		}
		horizontalSum = 0;
		currentSum = _mm256_setzero_ps();
	}
	return maxLine;
}

void copyMatrix(float dst[], float src[], int size) {
	for (int i = 0; i < size * size; i++) {
		dst[i] = src[i];
	}
}

void addMatrixMy(float A[], float B[], float result[], int size) {
	float* resTmp = new float[size * size] {};
	__m256* m256ResTmp = (__m256*)resTmp;
	__m256* m256A = (__m256*)A;
	__m256* m256B = (__m256*)B;
	for (int i = 0; i < size * size / 8; i++) {
		m256ResTmp[i] = _mm256_add_ps(m256A[i], m256B[i]);
	}
	copyMatrix(result, resTmp, size);
	delete[] resTmp;
}

void subMatrixMy(float A[], float B[], float result[], int size) {
	float* resTmp = new float[size * size] {};
	__m256* m256ResTmp = (__m256*)resTmp;
	__m256* m256A = (__m256*)A;
	__m256* m256B = (__m256*)B;
	for (int i = 0; i < size * size / 8; i++) {
		m256ResTmp[i] = _mm256_sub_ps(m256A[i], m256B[i]);
	}
	copyMatrix(result, resTmp, size);
	delete[] resTmp;
}

float* getMatrixBMy(float matrix[], int size) {
	float divider = findMaxColumnMy(matrix, size) * findMaxLineMy(matrix, size);
	float* matrixB = new float[size * size] {};
	float a = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			matrixB[j * size + i] = matrix[i * size + j] / divider;
		}
	}
	return matrixB;
}

void multMatrixMy(float a[], float b[], float result[], int size) {
	__m256* m256Result = (__m256*)result;
	const __m256* m256B = (const __m256*)b;
	__m256 m256A{};
	__m256 tmp;
	float* resTmp = new float[size * size] {};
	__m256* m256ResTmp = (__m256*)resTmp;
	for (int i = 0; i < size * size / 8; ++i)
		m256ResTmp[i] = _mm256_setzero_ps();
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			m256A = _mm256_set1_ps(a[size * i + j]);
			for (int k = 0; k < size / 8; k++)
			{
				tmp = _mm256_mul_ps(m256A, m256B[size * j / 8 + k]);
				m256ResTmp[size * i / 8 + k] = _mm256_add_ps(m256ResTmp[size * i / 8 + k], tmp);
			}
		}
	copyMatrix(result, resTmp, size);
	delete[] resTmp;
}

void multScalarMy(float A[], float scalar, int size) {
	__m256* m256A = (__m256*)A;
	__m256 m256Scalar = _mm256_set1_ps(scalar);
	for (int i = 0; i < size * size / 8; ++i)
		m256A[i] = _mm256_mul_ps(m256A[i], m256Scalar);
}

float* getMatrixRMy(float matrixA[], float matrixB[], int size) {
	float* matrixR = getOneMatrix(size);
	float* matrixMult = new float[size * size] {};
	multMatrixMy(matrixB, matrixA, matrixMult, size);
	subMatrixMy(matrixR, matrixMult, matrixR, size);
	delete[] matrixMult;
	return matrixR;
}

float* inverseMatrixMy(float matrix[], int size, int nSteps) {
	float* matrixB = getMatrixBMy(matrix, size);
	float* matrixR = getMatrixRMy(matrix, matrixB, size);
	float* matrixResult = getOneMatrix(size);
	float* matrixDegreeR = new float[size * size] {};
	copyMatrix(matrixDegreeR, matrixR, size);
	addMatrixMy(matrixResult, matrixDegreeR, matrixResult, size);
	for (int i = 1; i < nSteps; i++) {
		multMatrixMy(matrixDegreeR, matrixR, matrixDegreeR, size);
		addMatrixMy(matrixResult, matrixDegreeR, matrixResult, size);
	}
	multMatrixMy(matrixResult, matrixB, matrixResult, size);
	delete[] matrixB;
	delete[] matrixR;
	delete[] matrixDegreeR;
	return matrixResult;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
/// 
/// 

float getMaxSum(float* matrix, int size)
{
	float maxSumRow = FLT_MIN;
	float maxSumColumn = FLT_MIN;
	float sumRow = 0;
	float sumColumn = 0;
	for (int i = 0; i < size; i++) // rows
	{
		sumRow = 0;
		sumColumn = 0;
		for (int j = 0; j < size; j++) // columns
		{
			sumRow += fabs(matrix[size * i + j]);
			sumColumn += fabs(matrix[j * size + i]);
		}
		if (sumRow > maxSumRow) maxSumRow = sumRow;
		if (sumColumn > maxSumColumn) maxSumColumn = sumColumn;
	}
	return maxSumRow * maxSumColumn;
}

void fillB(float* matrix, float* B, int size)
{
	float max = getMaxSum(matrix, size);
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			B[size * i + j] = matrix[j * size + i] / max;
}

void multiply(float* A, float* B, float* result, int size)
{
	cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, size, size, size, 1.0, A, size, B, size, 0.0, result, size);
}

void subtract(float* A, float* B, float* result, int size) {
	cblas_saxpy(size * size, -1.0, B, 1, A, 1);
	cblas_scopy(size * size, A, 1, result, 1);
}

void addition(float* A, float* B, float* result, int size)
{
	cblas_saxpy(size * size, 1.0, B, 1, A, 1);
	cblas_scopy(size * size, A, 1, result, 1);
}

float* inverseMatrixBlas1(float matrix[], int size, int nSteps) {
	float* result = new float[size * size] {};
	float* R = new float[size * size] {};
	float* B = new float[size * size] {};
	float* I = getOneMatrix(size);
	float* tmp = new float[size * size] {};
	bool flag = true;
	fillB(matrix, B, size);
	multiply(B, matrix, tmp, size);
	subtract(I, tmp, R, size);
	addition(I, R, tmp, size);
	copyMatrix(result, R, size);
	for (int i = 2; i < nSteps; ++i) {
		multiply(flag ? result : I, R, flag ? I : result, size);
		addition(tmp, flag ? I : result, tmp, size);
		flag = !flag;
	}
	multiply(tmp, B, result, size);
	free(R);
	free(B);
	free(I);
	free(tmp);
	return result;
}

int main() 
{
	setlocale(LC_ALL, "");
	const int n = 2048;
	const int nSteps = 10;
	float* matrix = new float[n * n] {};
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			matrix[i * n + j] = i + j;
		}
	}
	double startTime = clock();
	//float* c = inverseMatrixMy(matrix, n, nSteps);
	//float* c = inverseMatrix(matrix, n, nSteps);
	float* c = inverseMatrixBlas(matrix, n, nSteps);
	double finishTime = clock();
	std::cout << "Время выполнения: " << (finishTime - startTime) / CLOCKS_PER_SEC << "c" << std::endl;
	delete[] matrix;
	delete[] c;
}

