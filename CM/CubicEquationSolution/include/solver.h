#ifndef SOLVER
#define SOLVER
#include <vector>

namespace solver{
    std::vector<int> solveEquation(float a, float b, float c, float d, float accuracy, double step);
    int calcDiscriminant(float a, float b, float c, std::vector<float> roots);
    void calcSquareRoots(float a, float b, float discriminant, std::vector<float> roots);
    int researchDerivative(float a, float b, float c, std::vector<float> derivativeRoots, bool *isNegative);
    int calcRoots(int maxRootsCount, float a, float b, float c, float d, double accuracy, double step, std::vector<float> roots);
    float bisectionMethod(double a, double b, double accuracy, double step);
}

#endif