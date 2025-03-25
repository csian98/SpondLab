#include <iostream>
#include <cmath>

double dfdx(double(*f)(double), double x, double h);

double integ(double(*f)(double), double x1, double x2, int N);
