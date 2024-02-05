#pragma optimize("O1", "on")
#include <iostream>
#include <intrin.h>
#include <math.h>
#include <unordered_map>
#define RAND_MAX 8388608

static unsigned long int next = 1;

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

void fillArrayStraight(unsigned int array[], unsigned int size)
{
    for (unsigned int i = 0; i < size - 1; i++) {
        array[i] = i + 1;
    }
    array[size - 1] = 0;
}

void fillArrayReverse(unsigned int array[], unsigned int size)
{
    for (unsigned int i = 0; i < size - 1; i++) {
        array[size - 1 - i] = size - 2 - i;
    }
    array[0] = size - 1;
}

int rand(void) {
    next = next * 1103515245 + 12345;
    return (unsigned int)(next) % (RAND_MAX + 1);
}

void fillArrayRandom(unsigned int array[], unsigned int size) {
    std::unordered_map<unsigned int, unsigned int> myMap;
    unsigned int j = rand() % (size - 1) + 1;
    unsigned int previouseIndex = 0;
    for (unsigned int i = 0; i < size - 1; i++) {
        while (myMap.find(j) != myMap.end()) {
            j = rand() % (size - 1) + 1;
        }
        myMap.emplace(j, 1);
        array[j] = previouseIndex;
        previouseIndex = j;
    }
    array[0] = previouseIndex;
}

unsigned int arrayTraversal(unsigned int array[], unsigned int size, unsigned int nRepetitions)
{
    unsigned __int64 tactsResult = UINT64_MAX;
    for (unsigned int i = 0; i < nRepetitions; i++) {
        unsigned __int64 curTactsResult = 0;
        unsigned int index = 0;
        unsigned __int64 tacts = __rdtsc();
        for (unsigned int j = 0; j < size; j++) {
            index = array[index];
        }
        curTactsResult += __rdtsc() - tacts;
        if (curTactsResult < tactsResult) {
            tactsResult = curTactsResult;
        }
        curTactsResult = 0;
    }
    return tactsResult / size;
}

unsigned int* arrayTraversalWithSizeChanging(unsigned int array[], unsigned int size, unsigned int lastSize, float step, int nRepetitions, int mode)
{
    int nSteps = log(lastSize / size) / log(1.2) + 1;
    unsigned int* tacts = new unsigned int[nSteps + 1] {};
    tacts[0] = nSteps + 1;
    for (int i = 0; i < nSteps; i++) {
        array = new unsigned int[size] {};
        if (mode == 0) {
            fillArrayStraight(array, size);
        }
        else if (mode == 1) {
            fillArrayReverse(array, size);
        }
        else {
            fillArrayRandom(array, size);
        }
        // warm up
        tacts[i + 1] = arrayTraversal(array, size, 1);
        // real work
        tacts[i + 1] = arrayTraversal(array, size, nRepetitions);
        std::cout << "size: " << size << ", tacts: " << tacts[i + 1] << std::endl;
        size *= step;
    }
    return tacts;
}

int main()
{
    // 32MB = 2^(23) * 4B
    unsigned int size = 256;
    int nRepetitions = 10;
    float step = 1.2;
    int mode = 1;
    unsigned int *array = new unsigned int[size];
    std::cout << warmUp(4) << std::endl;
    /*fillArrayRandom(array, size);
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << " ";
    }*/
    unsigned int *tacts = arrayTraversalWithSizeChanging(array, size, 8096 * 8096, step, nRepetitions, mode);
    std::cout << arrayTraversal(array, size, nRepetitions) << std::endl;
    for (int i = 0; i < tacts[0]; i++) {
        std::cout << tacts[i] << " ";
    }
}