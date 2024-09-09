#include <inputParser.h>
#include <solver.h>

bool inputParser::parseInput(int argc, char *argv[], double *a, double *b, double *c, double *d, double* accuracy, double* step)
{
    *a = solver::A;
    *b = solver::B;
    *c = solver::C;
    *d = solver::D;
    return false;
}
