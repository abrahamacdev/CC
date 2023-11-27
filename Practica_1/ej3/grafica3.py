
import matplotlib.pyplot as plt
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression
import numpy as np


if __name__ == "__main__":

    MAX_X = 5000
    MAX_Y = 10000

    # Valores reales
    x_real = np.linspace(1, MAX_X, 10)
    y_real = [32, 88, 204, 906, 1090, 1394, 1757, 2936, 4362, 9081]

    # Valores predichos para cada polinomio
    x_test = np.linspace(1, MAX_X, num=10000).reshape((-1, 1))
    polyRegressor = PolynomialFeatures((1, 4))      # Cada columna sería X^j
    y_pred = polyRegressor.fit_transform(x_test)[:, 1:]

    plt.plot(x_test, y_pred[:, 0], label='N')
    plt.plot(x_test, y_pred[:, 1], label='N^2')
    plt.plot(x_test, y_pred[:, 2], label='N^3')
    plt.plot(x_test, y_pred[:, 3], label='N^4')
    plt.plot(x_real, y_real, label='Real')
    plt.xlim(0, MAX_X)
    plt.ylim(0, MAX_Y)
    plt.legend()
    plt.savefig('ej3.png')
    plt.show()