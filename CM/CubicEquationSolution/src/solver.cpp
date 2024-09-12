#include "solver.h"
#include <iostream>
#include <cmath>

std::vector<double> solver::solveEquation(double a, double b, double c, double d, double accuracy, double step) {
    std::vector<double> derivativeRoots;
    std::vector<double> solutions;
    bool isNegative = false;
    discriminantState discriminantState = researchDerivative(a, b, c, derivativeRoots, &isNegative);
    calcRoots(discriminantState, a, b, c, d, accuracy, step, solutions);

    return std::vector<double>();
}

int solver::calcDiscriminant(double a, double b, double c, std::vector<double> roots) {
    return b * b - 4 * a * c;
}

void solver::calcSquareRoots(double a, double b, double discriminant, std::vector<double> roots) {
    roots.push_back((-1 * b + std::sqrt(discriminant)) / (2 * a));
    roots.push_back((-1 * b - std::sqrt(discriminant)) / (2 * a));
}

solver::discriminantState solver::researchDerivative(double a, double b, double c, std::vector<double> derivativeRoots, bool* isNegative) {
    a *= 3;
    b *= 2;
    int discriminant = calcDiscriminant(a, b, c, derivativeRoots);
    if (discriminant > 0) {
        calcSquareRoots(a, b, discriminant, derivativeRoots);
        return discriminantState::positive;
    } else if (discriminant == 0) {
        derivativeRoots.push_back(-1 * b / (2 * a));
        return discriminantState::zero;
    } else if (discriminant < 0) {
        return discriminantState::negative;
    }  
}

void solver::calcRoots(discriminantState discriminantState, double a, double b, double c, double d, double accuracy, double step, std::vector<double> roots) {
    if (discriminantState == discriminantState::negative) {
        double root;
        double segmentLeftBorder;
        // функция убывает, т.к. D < 0 и в 3ax^2 + 2bx + c, c < 0.
        if (c < 0) {
            if (d > 0) {
                segmentLeftBorder = findSegmentLeftBorder(0, true);
            } else {
                segmentLeftBorder = findSegmentLeftBorder(0, false); 
            }
        // функция возрастает.
        } else if (c > 0) {
            if (d > 0) {
                segmentLeftBorder = findSegmentLeftBorder(0, false);
            } else {
                segmentLeftBorder = findSegmentLeftBorder(0, true);
            }
        }
        root = bisectionMethod(segmentLeftBorder, segmentLeftBorder + solver::STEP, solver::ACCURACY);
        roots.push_back(root);
    }
}

double solver::findSegmentLeftBorder(double startPoint, bool rightDirection) {
    double funcValue = calcFunction(startPoint);
    double nextFuncValue = funcValue;
    double leftBorder = startPoint;
    if (rightDirection) {
        if (log) std::cout << "findSegmentLeftBorder right direction" << std::endl;
        leftBorder += solver::STEP;
        nextFuncValue = calcFunction(leftBorder);
        while (leftBorder <= solver::maxValue && funcValue * nextFuncValue > 0) {
            leftBorder += solver::STEP;
            funcValue = nextFuncValue;
            nextFuncValue = calcFunction(leftBorder);
        }
    } else {
        if (log) std::cout << "findSegmentLeftBorder left direction" << std::endl;
        leftBorder -= solver::STEP;
        nextFuncValue = calcFunction(leftBorder);
        while (leftBorder >= solver::minValue && funcValue * nextFuncValue > 0) {
            leftBorder -= solver::STEP;
            funcValue = nextFuncValue;
            nextFuncValue = calcFunction(leftBorder);
        }
        leftBorder -= solver::STEP;
    }
    if (log) std::cout << "found leftBorder: " << leftBorder << std::endl;
    if (leftBorder < solver::minValue || leftBorder > solver::maxValue) {
        if (log) std::cout << "leftBorder is out of range" << std::endl;
    }
    return leftBorder;
}

double solver::bisectionMethod(double a, double b, double accuracy) {
    bool isLeftMinus = false;
    if (a < 0) {
        isLeftMinus = true;
    }
    double midPoint = getSegmentMidpoint(a, b);
    double funcValue = calcFunction(midPoint);
    while (!isRoot(funcValue, solver::ACCURACY)) {
        if (isLeftMinus) {
            if (funcValue < 0) {
                a = midPoint;
            } else {
                b = midPoint;
            }
        } else {
            if (funcValue < 0) {
                b = midPoint;
            } else {
                a = midPoint;
            }
        }
        midPoint = getSegmentMidpoint(a, b);
        funcValue = calcFunction(midPoint);
    }
    return midPoint;
}

double solver::getSegmentMidpoint(double a, double b) {
    return (a + b) / 2;
}

bool solver::isRoot(double funcValue, double accuracy) {
    return std::abs(funcValue) <= accuracy;
}

double solver::calcFunction(double x) {
    return A * x * x * x + B * x * x + C * x + D;
}

void solver::researchRootsMultiplicity(std::vector<double> roots, std::vector<int> multiplicity) {
    for (int i = 0; i < roots.size(); i++) {
        double funcValue = calcFunction(roots.at(i));
        int curMultiplicity = 0;
        if (isRoot(funcValue, solver::ACCURACY)) {
            curMultiplicity++;
            funcValue = calcFirstDerivative(roots.at(i));
            if (isRoot(funcValue, solver::ACCURACY)) {
                curMultiplicity++;
                funcValue = calcSecondDerivative(roots.at(i));
                if (isRoot(funcValue, solver::ACCURACY)) {
                    curMultiplicity++;
                }
            }
        }
        multiplicity.push_back(curMultiplicity);
    }
} 

double solver::calcFirstDerivative(double x) {
    return solver::A * 3 * x * x + solver::B * 2 * x + solver::C;
}

double solver::calcSecondDerivative(double x) {
    return solver::A * 6 * x + solver::B * 2;
}