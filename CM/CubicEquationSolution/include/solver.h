#ifndef SOLVER
#define SOLVER
#include <vector>

namespace solver{

    enum discriminantState {
        negative = 0,
        zero = 1,
        positive = 2
    };

    double A = 0;
    double B = 0;
    double C = 0;
    double D = 0;
    double STEP = 0;
    double ACCURACY = 0;
    double minValue = -100000;
    double maxValue = 100000;

    bool log = true;


    std::vector<double> solveEquation(double a, double b, double c, double d, double accuracy, double step);
    int calcDiscriminant(double a, double b, double c, std::vector<double> roots);
    void calcSquareRoots(double a, double b, double discriminant, std::vector<double> roots);
    discriminantState researchDerivative(double a, double b, double c, std::vector<double> derivativeRoots, bool *isNegative);
    int calcRoots(discriminantState discriminantState, double a, double b, double c, double d, double accuracy, double step, std::vector<double> roots);
    double findSegmentLeftBorder(double startPoint, bool rightDirection);
    double bisectionMethod(double a, double b, double accuracy, double step, bool rightDirection);
    bool isRoot(double value, double accuracy);
    double calcFunction(double x);
}

#endif