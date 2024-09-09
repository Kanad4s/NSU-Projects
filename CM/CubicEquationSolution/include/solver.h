#ifndef SOLVER
#define SOLVER
#include <vector>

//если эдс направлена в сторону узла k, то берется со знаком плюс и с минусом иначе

namespace solver{
    std::vector<int> solveEquation(float a, float b, float c, float d);
    int calcDiscriminant(float a, float b, float c);
}

#endif