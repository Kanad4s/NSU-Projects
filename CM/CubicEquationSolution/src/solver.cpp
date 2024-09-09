#include "solver.h"

std::vector<int> solver::solveEquation(float a, float b, float c, float d)
{
    int discriminant = calcDiscriminant(3 * a, 2 * b, c);
    
    return std::vector<int>();
}

int solver::calcDiscriminant(float a, float b, float c)
{
    return b * b - 4 * a * c;
}
