#include "solver.h"
#include <cmath>

std::vector<int> solver::solveEquation(float a, float b, float c, float d, float accuracy, double step) {
    std::vector<float> derivativeRoots;
    int maxRootsCount = researchDerivative(3 * a, 2 * b, c, derivativeRoots);
    int res = calcRoots(maxRootsCount, a, b, c, d, accuracy);
    return std::vector<int>();
}

int solver::calcDiscriminant(float a, float b, float c, std::vector<float> roots) {
    return b * b - 4 * a * c;
}

void solver::calcSquareRoots(float a, float b, float discriminant, std::vector<float> roots) {
    roots.push_back((-1 * b + std::sqrt(discriminant)) / (2 * a));
    roots.push_back((-1 * b - std::sqrt(discriminant)) / (2 * a));
}

int solver::researchDerivative(float a, float b, float c, std::vector<float> derivativeRoots) {
    int discriminant = calcDiscriminant(a, b, c, derivativeRoots);
    if (discriminant < 0) {
        return 1;
    } else if (discriminant == 0) {
        derivativeRoots.push_back(-1 * b / (2 * a));
        return 2;
    } else if (discriminant > 0) {
        calcSquareRoots(a, b, discriminant, derivativeRoots);
        return 3;
    }
    return 0;
}

int solver::calcRoots(int maxRootsCount, float a, float b, float c, float d, double accuracy) {
    if (maxRootsCount == 1) {

    }
}
