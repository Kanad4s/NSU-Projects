#include "../include/solver.h"
#include <iostream>
#include <cmath>

double solver::A = 1;
double solver::B = 1;
double solver::C = 1;
double solver::D = 1;
double solver::STEP = 0.5;
double solver::ACCURACY = 0.02;
double solver::minValue = -100000;
double solver::maxValue = 100000;
bool solver::LOG = true;
bool solver::DEEP_LOG = true;
    
std::vector<double> solver::solveEquation(double a, double b, double c, double d, double accuracy, double step) {
    std::vector<double> derivativeRoots;
    std::vector<double> solutions;
    bool isNegative = false;
    discriminantState discriminantState = researchDerivative(a, b, c, derivativeRoots, &isNegative);
    if (LOG) std::cout << "derivative discriminantState: " << discriminantState << std::endl;
    std::cout << "derivativeRoots.size() outer:" << derivativeRoots.size() << std::endl;
    calcRoots(discriminantState, a, b, c, d, accuracy, step, solutions);
    return solutions;
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
    if (LOG) std::cout << "discriminant = " << discriminant << std::endl;
    if (discriminant > 0) {
        if (LOG) std::cout << "derivative discriminant > 0" << std::endl;
        calcSquareRoots(a, b, discriminant, derivativeRoots);
        return discriminantState::positive;
    } else if (discriminant == 0) {
        if (LOG) std::cout << "derivative discriminant == 0" << std::endl;
        derivativeRoots.push_back(-1 * b / (2 * a));
        return discriminantState::zero;
    } else if (discriminant < 0) {
        if (LOG) std::cout << "derivative discriminant < 0" << std::endl;
        return discriminantState::negative;
    }
    if (LOG) std::cout << "derivative discriminant is unexpected" << std::endl;
	return discriminantState::zero;
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
    } else if (discriminantState == discriminantState::zero) {
        
    }
}

//root may lie on the border

double solver::findSegmentLeftBorder(double startPoint, bool rightDirection) {
    double funcValue = calcFunction(startPoint);
    if (LOG) std::cout << "func value = " <<  funcValue << ", at x = " << startPoint << std::endl;
    double nextFuncValue = funcValue;
    double leftBorder = startPoint;
    if (LOG) std::cout << "direction: ";
    if (rightDirection) {
        if (LOG) std::cout << "right" << std::endl;
        leftBorder += solver::STEP;
        nextFuncValue = calcFunction(leftBorder);
        while (leftBorder < solver::maxValue && funcValue * nextFuncValue > 0) {
            if (LOG) std::cout << "func value: " << funcValue << ", next func value: " << nextFuncValue << std::endl;
            leftBorder += solver::STEP;
            funcValue = nextFuncValue;
            nextFuncValue = calcFunction(leftBorder);
        }
        leftBorder -= solver::STEP;
    } else {
        if (LOG) std::cout << "left" << std::endl;
        leftBorder -= solver::STEP;
        nextFuncValue = calcFunction(leftBorder);
        while (leftBorder > solver::minValue && funcValue * nextFuncValue > 0) {
            if (DEEP_LOG) std::cout << "func value: " << funcValue << ", next func value: " << nextFuncValue << std::endl;
            leftBorder -= solver::STEP;
            funcValue = nextFuncValue;
            nextFuncValue = calcFunction(leftBorder);
        }
    }
    if (LOG) std::cout << "Found segment:\n" << "\tfunc value: " << funcValue << ", next func value: " << nextFuncValue << std::endl;
    if (LOG) std::cout << "\tleftBorder: " << leftBorder << ", rightBorder: " << leftBorder + solver::STEP << std::endl;
    if (leftBorder < solver::minValue || leftBorder > solver::maxValue) {
        if (LOG) std::cout << "\tleftBorder is out of range" << std::endl;
    }
    return leftBorder;
}

double solver::bisectionMethod(double a, double b, double accuracy) {
    bool isLeftMinus = false;
    if (a < 0) {
        isLeftMinus = true;
    }
    if (LOG) std:: cout << "Bisection method:\n\tisLeftMinus: " << isLeftMinus << std::endl; 
    double midPoint = getSegmentMidpoint(a, b);
    double funcValue = calcFunction(midPoint);
    while (!isRoot(funcValue, solver::ACCURACY)) {
        if (DEEP_LOG) std::cout << "\tfuncValue: " << funcValue << " at midPoint: " << midPoint << std::endl;
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
    if (LOG) std::cout << "\tfound funcValue: " << funcValue << " at root: " << midPoint << std::endl;
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
