#ifndef SOLVER
#define SOLVER
#include <vector>

namespace solver{
    std::vector<int> solveEquation(float a, float b, float c, float d, float accuracy, double step);
    int calcDiscriminant(float a, float b, float c, std::vector<float> roots);
    int researchDerivative(float a, float b, float c, std::vector<float> derivativeRoots);
    int calcRoots(int maxRootsCount, float a, float b, float c, float d, double accuracy);
}

#endif