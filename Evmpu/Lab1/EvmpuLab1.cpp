#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>

long double function(long double x) {
    return (pow(M_E, x) * sin(x));
}

long double calculateTrapezoidIntegral(long long int n) {
    long double result = 0;
    long double section = M_PI / n;
    for (int i = 0; i < n; i++) {
        long double x1 = i * section;
        long double x2 = (i + 1) * section;
        result += (function(x1) + function(x2)) * 0.5;
    }
    return result * section;
}

int main(int argc, const char* argv[]) {
    long long int n = atof(argv[1]);
    printf("%.10Lf\n", calculateTrapezoidIntegral(n));
    return 0;
}
