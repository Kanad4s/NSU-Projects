#include <inputParser.h>
#include <solver.h>
#include <outputParser.h>
#include <iostream>

int main(int argc, char* argv[]) {
    float a, b, c, d;
    double accuracy;
    if (inputParser::parseInput(argc, argv, &a, &b, &c, &d, &accuracy)) {
        std::vector<int> solution = solver::solveEquation(a, b, c, d, accuracy);
        outputParser::showResults(solution);
    } else {
        std::cout << "not valid input" << std::endl;
        return 0;
    }
}