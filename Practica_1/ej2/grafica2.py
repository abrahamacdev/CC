
import matplotlib.pyplot as plt
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression
import numpy as np


if __name__ == "__main__":

    # Valores reales
    x_real = np.arange(1, 11)
    y_real = np.copy(x_real) + 1

    # Valores predichos para cada polinomio
    x_test = np.linspace(0, 10, num=1000).reshape((-1, 1))
    polyRegressor = PolynomialFeatures((1, 4))      # Cada columna sería X^j
    y_pred = polyRegressor.fit_transform(x_test)[:, 1:]

    plt.plot(x_test, y_pred[:, 0], label='N')
    plt.plot(x_test, y_pred[:, 1], label='N^2')
    plt.plot(x_test, y_pred[:, 2], label='N^3')
    plt.plot(x_test, y_pred[:, 3], label='N^4')
    plt.plot(x_real, y_real, label='Real')
    plt.xlim(0, 12)
    plt.ylim(0, 100)
    plt.legend()
    plt.savefig('ej2.png')
    plt.show()