#include "../include/inputParser.h"
#include "../include/solver.h"
#include "../include/outputParser.h"
#include <iostream>

int main(int argc, char* argv[]) {
    double a, b, c, d;
    if (inputParser::parseInput(argc, argv, &a, &b, &c, &d)) {
        std::vector<double> solution = solver::solveEquation(a, b, c, d);
        std::vector<int> multiplicity = solver::researchRootsMultiplicity(solution);
        outputParser::showResults(solution, multiplicity);
    } else {
        std::cout << "not valid input" << std::endl;
        return 0;
    }   
}
