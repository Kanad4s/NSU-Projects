#include "solver.h"
#include <cmath>

std::vector<int> solver::solveEquation(float a, float b, float c, float d, float accuracy, double step) {
    std::vector<float> derivativeRoots;
    std::vector<float> solutions;
    bool isNegative = false;
    int maxRootsCount = researchDerivative(a, b, c, derivativeRoots, &isNegative);
    int res = calcRoots(maxRootsCount, a, b, c, d, accuracy, step, solutions);
    return std::vector<int>();
}

int solver::calcDiscriminant(float a, float b, float c, std::vector<float> roots) {
    return b * b - 4 * a * c;
}

void solver::calcSquareRoots(float a, float b, float discriminant, std::vector<float> roots) {
    roots.push_back((-1 * b + std::sqrt(discriminant)) / (2 * a));
    roots.push_back((-1 * b - std::sqrt(discriminant)) / (2 * a));
}

int solver::researchDerivative(float a, float b, float c, std::vector<float> derivativeRoots, bool* isNegative) {
    a *= 3;
    b *= 2;
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

int solver::calcRoots(int maxRootsCount, float a, float b, float c, float d, double accuracy, double step, std::vector<float> roots) {
    if (maxRootsCount == 1) {
        float start, finish;
        // функция убывает, т.к. D < 0 и в 3ax^2 + 2bx + c, c < 0.
        if (c < 0) {
            if (d > 0) {
                bisectionMethod(0, __DBL_MAX__, accuracy, step);
            } else {
                bisectionMethod(__DBL_MIN__, 0, accuracy, step);
            }
        // функция возрастает.
        } else if (c > 0) {
            if (d > 0) {
                bisectionMethod(__DBL_MIN__, 0, accuracy, step);
            } else {
                bisectionMethod(0, __DBL_MAX__, accuracy, step);
            }
        } else {
            roots.push_back(0);
        }
    }
}

float solver::bisectionMethod(double a, double b, double accuracy, double step) {

}
