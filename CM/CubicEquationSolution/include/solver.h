#ifndef SOLVER
#define SOLVER
#include <vector>

namespace solver{

    enum discriminantState {
        negative = 0,
        zero = 1,
        positive = 2,
        unknown = 3
    };

    extern double A;
    extern double B;
    extern double C;
    extern double D;
    extern double STEP;
    extern double ACCURACY;
    extern double minValue;
    extern double maxValue;

    extern bool LOG;
    extern bool DEEP_LOG;

    std::vector<double> solveEquation(double a, double b, double c, double d);
    int calcDiscriminant(double a, double b, double c, std::vector<double> roots);
    void calcSquareRoots(double a, double b, double discriminant, std::vector<double> *roots);

    solver::discriminantState researchDerivative(double a, double b, double c, std::vector<double>* derivativeRoots, bool * isNegative);
    void calcRoots(discriminantState discriminantState, double a, double b, double c, double d, std::vector<double> *roots, std::vector<double> derivativeRoots);
    void solveNegativeDerivateDiscriminant(double c, double d, std::vector<double>* roots);
    void solvePositiveDerivativeDiscriminant(std::vector<double> *roots, std::vector<double> derivativeRoots);
    double findSegmentLeftBorder(double startPoint, bool rightDirection);
    double bisectionMethod(double a, double b);
    double getSegmentMidpoint(double a, double b);
    bool isRoot(double funcValue);
    double calcFunction(double x);
    std::vector<int> researchRootsMultiplicity(std::vector<double> roots);
    double calcFirstDerivative(double x);
    double calcSecondDerivative(double x);
    double myABS(double x);
}

#endif
