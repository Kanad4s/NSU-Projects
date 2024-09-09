#ifndef SOLVER
#define SOLVER
#include <vector>

namespace solver{
    std::vector<double> solveEquation(double a, double b, double c, double d, double accuracy, double step);
    int calcDiscriminant(double a, double b, double c, std::vector<double> roots);
    void calcSquareRoots(double a, double b, double discriminant, std::vector<double> roots);
    int researchDerivative(double a, double b, double c, std::vector<double> derivativeRoots, bool *isNegative);
    int calcRoots(int maxRootsCount, double a, double b, double c, double d, double accuracy, double step, std::vector<double> roots);
    double bisectionMethod(double a, double b, double accuracy, double step);
}

#endif