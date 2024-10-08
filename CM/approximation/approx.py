import matplotlib.pyplot as plt
import numpy as np
import argparse

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
        
    plt.legend()
    plt.show()
