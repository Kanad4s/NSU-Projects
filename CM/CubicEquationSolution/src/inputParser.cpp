#include "../include/inputParser.h"
#include "../include/solver.h"
#include <iostream>
#include "inputParser.h"

bool inputParser::parseInput(int argc, char *argv[], double *a, double *b, double *c, double *d)
{
    if (solver::LOG) std::cout << "arguments count: " << argc << ", arguments:" << std::endl;
    for (int i = 0; i < argc; i++) {
        if (solver::LOG) std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
    }
    if (argc == 6) {
        solver::B = atof(argv[1]);
        solver::C = atof(argv[2]);
        solver::D = atof(argv[3]);
        solver::STEP = atof(argv[4]);
        solver::ACCURACY = atof(argv[5]);
    } else {
        std::cout << "usage: ces-prog <B> <C> <D> <STEP> <ACCURACY>" << std::endl;
        return false;
    }
    *a = solver::A;
    *b = solver::B;
    *c = solver::C;
    *d = solver::D;
    return true;
}
