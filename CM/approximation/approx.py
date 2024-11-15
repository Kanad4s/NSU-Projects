import matplotlib.pyplot as plt
import numpy as np
import argparse

class CubicSpline:
    def __init__(self, x : list[float], y : list[float]):
        n = len(x)
        self.x_samples = x

        h = [0.0] + [(x[i] - x[i-1]) for i in range(1, n)]
        
        a = [h[i] for i in range(1, n-1)]
        b = [h[i+1] for i in range(1, n-1)]
        c = [2 * (h[i] + h[i+1]) for i in range(1, n-1)]

        f = [6 * ((y[i+1] - y[i]) / h[i+1] - (y[i] - y[i-1]) / h[i]) for i in range(1, n-1)]

        self.c = [0.0] + solveTridiagonalSlae(a, b, c, f) + [0.0]
        self.d = [0.0] + [(self.c[i] - self.c[i-1]) / h[i] for i in range(1, n)]
        self.b = [0.0] + [None] * (n-1)
        for i in range(1, n):
            self.b[i] = h[i] * self.c[i] / 2 - (h[i] ** 2) * self.d[i] / 6 + (y[i] - y[i-1]) / h[i]
            
        self.a = y

    @np.vectorize
    def __call__(self, x : float):
        # find interval to which x belongs
        i = np.searchsorted(self.x_samples, [x], side='left')[0]

        return self.a[i] + self.b[i] * (x - self.x_samples[i]) \
                + self.c[i] / 2 * (x - self.x_samples[i]) ** 2 \
                + self.d[i] / 6 * (x - self.x_samples[i]) ** 3


def solveTridiagonalSlae(a, b, c, f):
    n = len(a)
    alpha = [-b[0] / c[0]] + [None] * (n - 1)
    beta = [f[0] / c[0]] + [None] * (n - 1)
    # straight move
    for i in range(1, n):
        denominator = c[i] + a[i] * alpha[i-1]
        alpha[i] = -b[i] / denominator
        beta[i] = (f[i] - a[i] * beta[i-1]) / denominator

    # reverse stroke
    x = [None] * (n-1) + [beta[n-1]]
    for i in range(n-2, -1, -1):
        x[i] = alpha[i] * x[i+1] + beta[i]

    return x


class NewtonPolynomial:
    def __init__(self, x : list[float], y : list[float]):
        self.dividedDiffs = []
        for i in range(len(x)):
            self.dividedDiffs += [dividedDifference(x[0:i+1], y[0:i+1])]
        self.x = x
        self.n = len(x)
    
    @np.vectorize
    def __call__(self, x : float) -> float:
        ret = 0
        for i in range(self.n):
            numerator = 1
            for j in range(i):
                numerator *= x - self.x[j]
            ret += self.dividedDiffs[i] * numerator
        return ret
        
        
def dividedDifference(x : list[float], y : list[float]) -> float:
    ret = 0
    for i in range(len(x)):
        divider = 1
        for j in range(len(x)):
            if i != j:
                divider *= x[i] - x[j]
        ret += y[i] / divider
    return ret
    

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Approximate smthg')
    parser.add_argument('n', type=int, default=10, help='number of interpolation nodes')
    args = parser.parse_args()
    print(args.n)
    
    plt.xlabel('x label')
    plt.ylabel('y label')
    plt.title("Approximation")
    plt.xlim(-1, 1)
    plt.ylim(-0.5, 1.5)
    
    sample = np.linspace(-1, 1, 200)
    plt.plot(sample, abs(sample), label='y = |x|')

    # for n in [1, 2, 4, 10, 20]:
    x = np.linspace(-1, 1, args.n) 
    y = abs(x)

    polynom = NewtonPolynomial(x, y)

    plt.plot(sample, polynom(polynom, sample), label= f"NewtonPolynomial (n={args.n})")

    spline = CubicSpline(x, y)

    plt.plot(sample, spline(spline, sample), label= f"CubicSpline (n={args.n})")
        
    plt.legend()
    plt.show()
