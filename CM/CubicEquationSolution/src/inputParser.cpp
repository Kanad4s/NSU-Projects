#include "../include/inputParser.h"
#include "../include/solver.h"
#include <iostream>

bool inputParser::parseInput(int argc, char *argv[], double *a, double *b, double *c, double *d, double* accuracy, double* step)
{
   
    std::cout << argc << std::endl;
    *a = solver::A;
    *b = solver::B;
    *c = solver::C;
    *d = solver::D;
    return true;
}
