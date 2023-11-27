
import matplotlib.pyplot as plt
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression
import numpy as np


if __name__ == "__main__":

    MAX_X = 30
    MAX_Y = 436*10^6

    # Valores reales
    x_real = np.arange(1, MAX_X+1)
    y_real = np.ones((MAX_X,)) * np.inf
    y_real[0:11] = [8, 11, 24, 89, 466, 3327, 28175, 260039, 2866567, 33604228, 435000891]
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
    plt.savefig('normales.png')
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
    plt.savefig('normales_log.png')
    plt.show()