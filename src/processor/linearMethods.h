#ifndef LINEAR_METHODS_H
#define LINEAR_METHODS_H

#include <QtWidgets>

namespace LinearMethods {

    template<class T>
    /*!
      Решение системы уравнений методом Гаусса вида AX=b.
     \param A - матрица A
     \param b - вектор b
     \param eps - точность, при которой значение равно 0, если меньше её.
     \return QVector<T>
    */
    QVector<T> gauss(QVector<QVector<T>> A, QVector<T> b, double eps = 1e-11);

}

#include "linearMethods.hpp"

#endif // LINEAR_METHODS_H
