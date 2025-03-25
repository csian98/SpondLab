#include "calculus.hpp"

double dfdx(double(*f)(double), double x, double h){
  return (f(x+h)-f(x-h))/h*0.5;
}

double integ(double(*f)(double), double x1, double x2, int N=1000){
  double sum=0;
  double h=(x2-x1)/N;
  double xx=x1;
  for(int i=0; i<N; ++i){
    sum+=(f(xx)+f(xx+h));
    xx+=h;
  }
  sum*=0.5*h;
  return sum;
}
