#include "../include/inputParser.h"
#include "../include/solver.h"
#include "../include/outputParser.h"
#include <iostream>

// фаза тока опережает фазу напряжения
// в синусоидальной фаза тока отстает от напряжения
// если эдс направлена в сторону узла k, то берется со знаком плюс и с минусом иначе

int main(int argc, char* argv[]) {
    double aa, ab, ac, ad;
    double accuracy, step;
    if (inputParser::parseInput(argc, argv, &aa, &ab, &ac, &ad, &accuracy, &step)) {
        std::vector<double> solution = solver::solveEquation(aa, ab, ac, ad, accuracy, step);
        outputParser::showResults(solution);
    } else {
        std::cout << "not valid input" << std::endl;
        return 0;
    }   
}
