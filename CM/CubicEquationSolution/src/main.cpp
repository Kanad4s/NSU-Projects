#include "../include/inputParser.h"
#include "../include/solver.h"
#include "../include/outputParser.h"
#include <iostream>

int main(int argc, char* argv[]) {
    double a, b, c, d;
    double step;
    if (inputParser::parseInput(argc, argv, &a, &b, &c, &d, &step)) {
        std::vector<double> solution = solver::solveEquation(a, b, c, d, step);
        outputParser::showResults(solution);
    } else {
        std::cout << "not valid input" << std::endl;
        return 0;
    }   
}
