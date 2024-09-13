#include <inputParser.h>
#include <solver.h>
#include <iostream>

bool inputParser::parseInput(int argc, char *argv[], double *a, double *b, double *c, double *d, double* accuracy, double* step)
{
    std::cout << argv << std::endl;
    *a = solver::A;
    *b = solver::B;
    *c = solver::C;
    *d = solver::D;
    return false;
}
