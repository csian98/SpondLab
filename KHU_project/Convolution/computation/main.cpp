#include <iostream>
#include <cmath>
#include <cassert>
#include "matrix.hpp"

// dxdt=-x**3+sin(t)
double f(double x, double t){
  double pow_3=std::pow(x, 3);
  return -pow_3+std::sin(t);
}

template <typename T>
void ode(double(*f)(double, double), Vector<T>& dep, Vector<T>& indep){
  assert(dep.get_dim()==indep.get_dim());
  int dim=dep.get_dim();
  double c1, c2, c3, c4, tau;
  tau=indep[1]-indep[0];
  for(int i=0; i<dim-1; ++i){
    c1=tau*f(dep[i], indep[i]);
    c2=tau*f(dep[i]+c1/2, indep[i]+tau/2);
    c3=tau*f(dep[i]+c2/2, indep[i]+tau/2);
    c4=tau*f(dep[i]+c3, indep[i]+tau);
    dep[i+1]=dep[i]+(c1+2*c2+2*c3+c4)/6;
  }
}

int main(void){
  // dependent time variables
  Vector<double> time(0, 10, 10000);
  Vector<double> y(time.get_dim());
  y[0]=0;
  double h=time[1]-time[0];

  ode(f, y, time);

  y.print();

  return 0;
}
