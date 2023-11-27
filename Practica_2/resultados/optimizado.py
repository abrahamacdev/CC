
import matplotlib.pyplot as plt
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression
import numpy as np


if __name__ == "__main__":

    MAX_X = 30
    MAX_Y = 5000 #1*10**6

    print(MAX_Y)

    # Valores reales
    x_real = np.arange(1, MAX_X+1)
    y_real = np.ones((MAX_X,)) * np.inf
    y_real[0:11] = [6, 18, 48, 216, 1281, 8206, 31748, 269140, 2901361, 34109091, 435730094]
    y_real_log = np.log(y_real)

    print(x_real)
    print(y_real)

    # Valores predichos para cada polinomio
    x_test = np.linspace(1, MAX_X, num=10000).reshape((-1, 1))
    polyRegressor = PolynomialFeatures((1, 3))      # Cada columna sería X^j
    y_pred = polyRegressor.fit_transform(x_test)[:, 1:]

    plt.plot(x_test, y_pred[:, 0], label='N')
    plt.plot(x_test, y_pred[:, 1], label='N^2')
    plt.plot(x_test, y_pred[:, 2], label='N^3')
    plt.plot(x_test, np.power(2, y_pred[:, 0]), label='2^N')
    plt.plot(x_test, np.power(10, y_pred[:, 0]), label='10^N')
    plt.plot(x_real, y_real, label='Real')
    plt.xlim(0, MAX_X)
    plt.ylim(0, MAX_Y)
    plt.legend()
    plt.savefig('optimizado.png')
    plt.show()

    plt.plot(x_test, np.log(y_pred[:, 0]), label='N')
    plt.plot(x_test, np.log(y_pred[:, 1]), label='N^2')
    plt.plot(x_test, np.log(y_pred[:, 2]), label='N^3')
    plt.plot(x_test, np.log(np.power(2, y_pred[:, 0])), label='2^N')
    plt.plot(x_test, np.log(np.power(10, y_pred[:, 0])), label='10^N')
    plt.plot(x_real, y_real_log, label='Real')
    plt.xlim(0, MAX_X)
    plt.ylim(0, np.log(MAX_Y))
    plt.legend()
    plt.savefig('optimizado_log.png')
    plt.show()