#include "solver.h"

std::vector<int> solver::solveEquation(float a, float b, float c, float d, float accuracy)
{
    int discriminant = calcDiscriminant(3 * a, 2 * b, c);
    if (discriminant < 0) {

    } else if (discriminant == 0) {

    } else {

    }
    return std::vector<int>();
}

int solver::calcDiscriminant(float a, float b, float c)
{
    return b * b - 4 * a * c;
}
