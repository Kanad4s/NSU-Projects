import numpy as np
import matplotlib.pyplot as plt

def cubicSpline(x, y):
    n = len(x) - 1
    h = np.diff(x)

    A = np.zeros((n + 1, n + 1))
    f = np.zeros(n + 1)

    A[0, 0] = 1
    A[n, n] = 1

    for i in range(1, n):
        A[i, i - 1] = h[i - 1]
        A[i, i] = 2 * (h[i - 1] + h[i])
        A[i, i + 1] = h[i]
        f[i] = 3 * ((y[i + 1] - y[i]) / h[i] - (y[i] - y[i - 1]) / h[i - 1])

    # Метод Гаусса
    for i in range(n + 1):
        for j in range(i + 1, n + 1):
            r = A[j, i] / A[i, i]
            A[j, i:] -= r * A[i, i:]
            f[j] -= r * f[i]

    c = np.zeros(n + 1)
    for i in range(n, -1, -1):
        c[i] = (f[i] - np.dot(A[i, i + 1:], c[i + 1:])) / A[i, i]

    a = y
    b = np.zeros(n)
    d = np.zeros(n)

    for i in range(n):
        b[i] = (y[i + 1] - y[i]) / h[i] - (h[i] * (c[i + 1] + 2 * c[i])) / 3
        d[i] = (c[i + 1] - c[i]) / (3 * h[i])

    return a[:-1], b, c[:-1], d


def calcSpline(x_new, x, a, b, c, d):
    n = len(x) - 1
    y_new = np.zeros_like(x_new)

    for i in range(n):
        mask = (x_new >= x[i]) & (x_new <= x[i + 1])
        if np.any(mask):
            dx = x_new[mask] - x[i]
            y_new[mask] = a[i] + b[i] * dx + c[i] * dx ** 2 + d[i] * dx ** 3

    return y_new

if __name__ == "__main__":
    x = np.array([-1, 0, 2, 3, 5])
    y = np.array([1, 2, 4, 1, -3])
    a, b, c, d = cubicSpline(x, y)

    xNew = np.linspace(np.min(x), np.max(x), 100)
    yNew = calcSpline(xNew, x, a, b, c, d)

    plt.plot(xNew, yNew, label='function')
    plt.title("Approximation")
    xBounds = [-2, 6]
    yBounds = [-4, 5]
    plt.xlim(xBounds[0], xBounds[1])
    plt.ylim(yBounds[0], yBounds[1])
    plt.legend()
    plt.xlabel('x')
    plt.ylabel('y')
    plt.grid()
    plt.show()