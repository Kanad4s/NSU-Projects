#ifndef SOLVER
#define SOLVER
#include <vector>

namespace solver{

    enum discriminantState {
        negative = 0,
        zero = 1,
        positive = 2
    };

    extern double A;
    extern double B;
    extern double C;
    extern double D;
    extern double STEP;
    extern double ACCURACY;
    extern double minValue;
    extern double maxValue;

    extern bool log;



    std::vector<double> solveEquation(double a, double b, double c, double d, double accuracy, double step);
    int calcDiscriminant(double a, double b, double c, std::vector<double> roots);
    void calcSquareRoots(double a, double b, double discriminant, std::vector<double> roots);
    discriminantState researchDerivative(double a, double b, double c, std::vector<double> derivativeRoots, bool *isNegative);
    void calcRoots(discriminantState discriminantState, double a, double b, double c, double d, double accuracy, double step, std::vector<double> roots);
    double findSegmentLeftBorder(double startPoint, bool rightDirection);
    double bisectionMethod(double a, double b, double accuracy);
    double getSegmentMidpoint(double a, double b);
    bool isRoot(double funcValue, double accuracy);
    double calcFunction(double x);
    void researchRootsMultiplicity(std::vector<double> roots, std::vector<int> multiplicity);
    double calcFirstDerivative(double x);
    double calcSecondDerivative(double x);
}

#endif
