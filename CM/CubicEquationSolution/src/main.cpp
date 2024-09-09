#include <inputParser.h>
#include <solver.h>
#include <outputParser.h>
#include <iostream>

int main(int argc, char* argv[]) {
    float a, b, c, d;
    if (inputParser::parseInput(argc, argv, &a, &b, &c, &d)) {
        std::vector<int> solution = solver::solveEquation(a, b, c, d);
        outputParser::showResults(solution);
    } else {
        std::cout << "not valid input" << std::endl;
        return 0;
    }
}