#include <iostream>
#include <intrin.h>
#include <vector>

namespace {
    const int offset = 1024 * 1024 * 4;
    const int arraySize = 1024 * 1024 * 4;
}
int warmUp(int seconds)
{
    int size = 1024;
    std::vector<int> a(size);
    std::vector<int> b(size);
    for (int l = 0; l < seconds; l++) {
        for (int k = 0; k < 64; k++) {
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    a[j] = i * j + 1;
                    b[j] = a[i] * j;
                    a[j] *= b[j] / 2;
                }
            }
        }
    }
    if (a[size / 2] == b[size / 2]) {
        return 1;
    }
    else {
        return 0;
    }
}

void fillArray(int array[], int size, int ways)
{
    //32KB / 8 = 4KB
    //4KB / 4B = 1024
    int sizeOfWay = size / ways;
    for (int j = 0; j < sizeOfWay; j++) {
        for (int i = 1; i <= ways; i++) {
            array[(i - 1) * offset + j] = offset * i + j;
        }
    }
    for (int i = 0; i < sizeOfWay; i++) {
        array[offset * (ways - 1) + i] = (1 + i) % sizeOfWay;
    }
}

std::vector<double> arrayTraversal(int array[], int size, int maxWays, int nRepetitions)
{
    std::vector<double> times(maxWays);
    for (int i = 1; i <= maxWays; i++) {
        unsigned __int64 curResult = 0;
        unsigned __int64 result = UINT64_MAX;
        fillArray(array, size, i);
        int k = 0;
        for (int j = 0; j < size; j++) {
            k = array[k];
        }
        curResult = __rdtsc();
        for (int r = 0; r < nRepetitions; r++) {
            k = 0;
            //curResult = __rdtsc();
            for (int j = 0; j < size; j++) {
                k = array[k];
            }
            /*curResult = __rdtsc() - curResult;
            if (curResult < result)
                result = curResult;*/
        }
        result = __rdtsc() - curResult;
        times[i - 1] = result / size / nRepetitions;
    }
    return times;
}

int main()
{
    // 32KB / 4B = 32 * 1024 / 4 = 8 * 1024
    //int size = 1024 * 1024 * 16 * 32;//1024 * 1024 * 512
    int size = offset * 32;
    int realElementsNumber = arraySize;
    int maxWays = 32;
    int nRepetitions = 4;
    int* array = new int[size] {};
    for (int i = 0; i < size; i++) {
        array[i] = 0;
    }
    /*fillArray2(array, size, 8);
    for (int i = 0; i < size; i++) {
        std::cout << i << " = " << array[i] << " \n";
    }*/
    //std::cout << std::endl;

    std::cout << warmUp(2) << std::endl;

    std::vector<double> times = arrayTraversal(array, realElementsNumber, maxWays, nRepetitions);
    for (int i = 0; i < times.size(); i++) {
        std::cout << i + 1 << " ways - " << times[i] << " " << std::endl;
    }
    delete[] array;
}
