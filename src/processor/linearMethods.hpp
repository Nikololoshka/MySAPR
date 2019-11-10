#include "linearMethods.h"

template<class T>
QVector<T> LinearMethods::gauss(QVector<QVector<T> > A, QVector<T> b, double eps)
{
    const int SIZE = b.size();
    QVector<T> res(SIZE);
    T max;

    int k = 0;
    int index = 0;
    while (k < SIZE) {
        // поиск максимального элемента
        max = qAbs(A[k][k]);
        index = k;

        for (int i = k + 1; i < SIZE; ++i) {
            if (qAbs(A[i][k]) > max) {
                max = qAbs(A[i][k]);
                index = i;
            }
        }

        for (int j = 0; j < SIZE; ++j) {
            qSwap(A[k][j], A[index][j]);
        }
        qSwap(b[k], b[index]);

        // нормализация уравнения
        for (int i = k; i < SIZE; ++i) {
            T t = A[i][k];
            // не нулевой коэффициент
            if (qAbs(t) > eps) {
                for (int j = 0; j < SIZE; ++j) {
                    A[i][j] /= t;
                }
                b[i] /= t;

                // не сама строка
                if (i != k) {
                    for (int j = 0; j < SIZE; ++j) {
                        A[i][j] -= A[k][j];
                    }
                    b[i] -= b[k];
                }
            }
        }
        k++;
    }

    // обратная подстановка
    for (int i = SIZE - 1; i >= 0; --i) {
        res[i] = b[i];
        for (int j = 0; j < i; ++j) {
            b[j] -= A[j][i] * res[i];
        }
    }

    return res;
}
