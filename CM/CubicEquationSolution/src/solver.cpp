#include "../include/solver.h"
#include <iostream>
#include <cmath>
#include "solver.h"

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

std::vector<double> solver::solveEquation(double a, double b, double c, double d) {
    std::vector<double> derivativeRoots;
    std::vector<double> solutions;
    bool isNegative = false;
    discriminantState discriminantState = researchDerivative(a, b, c, &derivativeRoots, &isNegative);
    if (LOG) std::cout << "derivative discriminantState: " << discriminantState << std::endl;
    calcRoots(discriminantState, c, d, &solutions, derivativeRoots);
    return solutions;
}

int solver::calcDiscriminant(double a, double b, double c, std::vector<double> roots) {
    return b * b - 4 * a * c;
}

void solver::calcSquareRoots(double a, double b, double discriminant, std::vector<double>* roots) {
    double alpha = (-1 * b - std::sqrt(discriminant)) / (2 * a);
    double beta = (-1 * b + std::sqrt(discriminant)) / (2 * a);
    if (LOG) std::cout << "CalcSquareRoots:\n\talpha: " << alpha << "\n\tbeta: " << beta << std::endl;
    if (beta < alpha) {
        std::swap(alpha, beta);
        if (LOG) std::cout << "\tswap: alpha: " << alpha << ", beta: " << beta << std::endl;
    }
    roots->push_back(alpha);
    roots->push_back(beta);
}

solver::discriminantState solver::researchDerivative(double a, double b, double c, std::vector<double>* derivativeRoots, bool* isNegative) {
    a *= 3;
    b *= 2;
    int discriminant = calcDiscriminant(a, b, c, *derivativeRoots);
    if (LOG) std::cout << "discriminant = " << discriminant << std::endl;
    if (discriminant > solver::ACCURACY) {
        if (LOG) std::cout << "derivative discriminant > ACCURACY" << std::endl;
        calcSquareRoots(a, b, discriminant, derivativeRoots);
        return discriminantState::positive;
    } else if (abs(discriminant) <= solver::ACCURACY) {
        if (LOG) std::cout << "derivative |discriminant| <= ACCURACY" << std::endl;
        derivativeRoots->push_back(-1 * b / (2 * a));
        return discriminantState::zero;
    } else if (discriminant < -solver::ACCURACY) {
        if (LOG) std::cout << "derivative discriminant < -ACCURACY" << std::endl;
        return discriminantState::negative;
    }
    if (LOG) std::cout << "derivative discriminant is unexpected" << std::endl;
	return discriminantState::unknown;
}

void solver::calcRoots(discriminantState discriminantState, double c, double d, std::vector<double>* roots,
                        std::vector<double> derivativeRoots) {
    double segmentLeftBorder;
    double root;
    if (LOG) std::cout << "Calc roots:" << std::endl;
    if (discriminantState == discriminantState::negative || discriminantState == discriminantState::zero) {
        solveNegativeDerivateDiscriminant(c, d, roots);
    } else if (discriminantState == discriminantState::positive) {
        solvePositiveDerivativeDiscriminant(roots, derivativeRoots);
    }
}

void solver::solveNegativeDerivateDiscriminant(double c, double d, std::vector<double> *roots) {
    if (LOG) std::cout << "solveNegativeDerivateDiscriminant" << std::endl;
    double root, segmentLeftBorder;
    bool isIncreasing = isFuncIncreasing();
    if (LOG) std::cout << "isIncreasing func: " << isIncreasing << std::endl;
    // функция убывает, т.к. D < ACCURACY и в 3ax^2 + 2bx + c, c < ACCURACY.
    if (!isIncreasing) {
        if (LOG) std::cout << "function is downing" << std::endl;
        if (d > solver::ACCURACY) {
            segmentLeftBorder = findSegmentLeftBorder(0, true);
        } else {
            segmentLeftBorder = findSegmentLeftBorder(0, false); 
        }
    // функция возрастает.
    } else if (isIncreasing) {
        if (LOG) std::cout << "function is increasing" << std::endl;
        if (d > solver::ACCURACY) {
            segmentLeftBorder = findSegmentLeftBorder(0, false);
        } else {
            segmentLeftBorder = findSegmentLeftBorder(0, true);
        }
    }
    root = bisectionMethod(segmentLeftBorder, segmentLeftBorder + solver::STEP);
    roots->push_back(root);
    for (int i = 0; i < roots->size(); i++) {
        if (LOG) std::cout << "\tpushed root: " << roots->at(0) << std::endl;
    }
}

bool solver::isFuncIncreasing() {
    for (int i = 0; i < 4; i++) {
        double derivativeValue = calcFirstDerivative(i);
        if (derivativeValue > 0) {
            return true;
        } else if (derivativeValue < 0) {
            return false;
        }
    }
    return true;
}

void solver::solvePositiveDerivativeDiscriminant(std::vector<double>* roots, std::vector<double> derivativeRoots) {
    if (LOG) std::cout << "solvePositiveDerivativeDiscriminant" << std::endl;
    double alpha = derivativeRoots.at(0);
    double beta = derivativeRoots.at(1);
    double funcAtAlpha = calcFunction(alpha);
    double funcAtBeta = calcFunction(beta);
    double root, segmentLeftBorder;
    if (LOG) std::cout << "\tfunc at alpha: " << funcAtAlpha << ", func at beta: " << funcAtBeta << std::endl;
    if (LOG) std::cout << "\tACCURACY: " << solver::ACCURACY << "abs value alpha: " << abs(funcAtAlpha) << ", beta: " << abs(funcAtBeta) << std::endl;
    if (LOG) std::cout << "\tevent: ";
    if (funcAtAlpha <= -solver::ACCURACY && funcAtBeta <= -solver::ACCURACY) {
        if (LOG) std::cout << "f(Alpha) <= -ACCURACY && f(Beta) <= -ACCURACY" << std::endl;
        segmentLeftBorder = findSegmentLeftBorder(beta, true);
    } else if (funcAtAlpha >= solver::ACCURACY && funcAtBeta >= solver::ACCURACY) {
        if (LOG) std::cout << "f(Alpha) >= ACCURACY && f(Beta) >= ACCURACY" << std::endl;
        segmentLeftBorder = findSegmentLeftBorder(alpha, false);
    } else if (myABS(funcAtAlpha) <= solver::ACCURACY && funcAtBeta <= -solver::ACCURACY) {
        if (LOG) std::cout << "|f(Alpha)| <= ACCURACY && f(Beta) <= -ACCURACY" << std::endl;
        roots->push_back(alpha);
        segmentLeftBorder = findSegmentLeftBorder(beta, true);
    } else if (funcAtAlpha >= solver::ACCURACY && myABS(funcAtBeta) <= solver::ACCURACY) {
        if (LOG) std::cout << "f(Alpha) >= ACCURACY && |f(Beta)| <= ACCURACY" << std::endl;
        roots->push_back(beta);
        segmentLeftBorder = findSegmentLeftBorder(alpha, false);
    } else if (funcAtAlpha >= solver::ACCURACY && funcAtBeta <= -solver::ACCURACY) {
        if (LOG) std::cout << "f(Alpha) >= ACCURACY && f(Beta) <= -ACCURACY" << std::endl;
        segmentLeftBorder = findSegmentLeftBorder(alpha, false);
        root = bisectionMethod(segmentLeftBorder, segmentLeftBorder + solver::STEP);
        roots->push_back(root);
        segmentLeftBorder = findSegmentLeftBorder(beta, true);
        root = bisectionMethod(segmentLeftBorder, segmentLeftBorder + solver::STEP);
        roots->push_back(root);
        segmentLeftBorder = findSegmentLeftBorder(alpha, true);
    } else if (myABS(funcAtAlpha) <= solver::ACCURACY && myABS(funcAtBeta) <= solver::ACCURACY) {
        if (LOG) std::cout << "|f(Alpha)| <= ACCURACY && |f(Beta)| <= ACCURACY" << std::endl;
        root = (alpha + beta) / 2;
        roots->push_back(root);
        return;
    }
    root = bisectionMethod(segmentLeftBorder, segmentLeftBorder + solver::STEP);
    roots->push_back(root);
}

double solver::findSegmentLeftBorder(double startPoint, bool rightDirection) {
    double funcValue = calcFunction(startPoint);
    if (LOG) std::cout << "findSegmentLeftBorder\n\tfunc value = " <<  funcValue << ", at x = " << startPoint << std::endl;
    double nextFuncValue = funcValue;
    double leftBorder = startPoint;
    if (LOG) std::cout << "\tdirection: ";
    if (rightDirection) {
        if (LOG) std::cout << "right" << std::endl;
        leftBorder += solver::STEP;
        nextFuncValue = calcFunction(leftBorder);
        while (leftBorder < solver::maxValue && funcValue * nextFuncValue > 0) {
            if (LOG) std::cout << "\tfunc value: " << funcValue << ", next func value: " << nextFuncValue << std::endl;
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
            if (DEEP_LOG) std::cout << "\tfunc value: " << funcValue << ", next func value: " << nextFuncValue << std::endl;
            leftBorder -= solver::STEP;
            funcValue = nextFuncValue;
            nextFuncValue = calcFunction(leftBorder);
        }
    }
    if (LOG) std::cout << "\tFound segment:\n" << "\t\tfunc value: " << funcValue << ", next func value: " << nextFuncValue << std::endl;
    if (LOG) std::cout << "\t\tleftBorder: " << leftBorder << ", rightBorder: " << leftBorder + solver::STEP << std::endl;
    if (leftBorder < solver::minValue || leftBorder > solver::maxValue) {
        if (LOG) std::cout << "\t\tleftBorder is out of range" << std::endl;
    }
    return leftBorder;
}

double solver::bisectionMethod(double a, double b) {
    if (LOG) std::cout << "Bisection method on [" << a << ";" << b << "]" << std::endl;
    if (isRoot(calcFunction(a))) {
        if (LOG) std::cout << "\troot is a: " << a << std::endl;
        return a;
    } else if (isRoot(calcFunction(b))) {
        if (LOG) std::cout << "\troot is b: " << b << std::endl;
        return b;
    }
    bool isLeftMinus = false;
    if (calcFunction(a) < 0) {
        isLeftMinus = true;
    }
    if (LOG) std::cout << "\tisLeftMinus: " << isLeftMinus << std::endl;
    double midPoint = getSegmentMidpoint(a, b);
    double funcValue = calcFunction(midPoint);
    while (!isRoot(funcValue)) {
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

bool solver::isRoot(double funcValue) {
    return std::abs(funcValue) <= solver::ACCURACY;
}

double solver::calcFunction(double x) {
    return A * x * x * x + B * x * x + C * x + D;
}

std::vector<int> solver::researchRootsMultiplicity(std::vector<double> roots) {
    std::vector<int> multiplicity;
    for (int i = 0; i < roots.size(); i++) {
        double funcValue = calcFirstDerivative(roots.at(i));
        int curMultiplicity = 1;
        if (isRoot(funcValue)) {
            curMultiplicity++;
            funcValue = calcSecondDerivative(roots.at(i));
            if (isRoot(funcValue)) {
                curMultiplicity++;
            }
        }
        multiplicity.push_back(curMultiplicity);
    }
    return multiplicity;
} 

double solver::calcFirstDerivative(double x) {
    return solver::A * 3 * x * x + solver::B * 2 * x + solver::C;
}

double solver::calcSecondDerivative(double x) {
    return solver::A * 6 * x + solver::B * 2;
}

double solver::myABS(double x) {
    if (x < 0) {
        x *= -1;
    }
    return x;
}
