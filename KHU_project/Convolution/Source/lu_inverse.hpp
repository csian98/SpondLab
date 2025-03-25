#ifndef LU_INVERSE_HPP
#define LU_INVERSE_HPP

#include <iostream>
#include <memory>
#include <cassert>
#include <algorithm>
#include <cstdlib>

#include "matrix.hpp"
#include "inverse.hpp"

template <typename T>
void LU_decomposition(Matrix<T>& A, Matrix<T>& L, Matrix<T>& U){
  int dim=A.get_dim1();
  
  for(int i=0; i<dim; ++i){
    for(int j=0; j<dim; ++j){
      if(j<i) L[j][i]=0;
      else{
        L[j][i]=A[j][i];
        for(int k=0; k<i; ++k){
          L[j][i]=L[j][i]-L[j][k]*U[k][i];
        }
      }
    }
    for(int j=0; j<dim; ++j){
      if(j<i) U[i][j]=0;
      else if(j==i) U[i][j]=1;
      else{
        U[i][j]=A[i][j]/L[i][i];
        for(int k=0; k<i; ++k){
          U[i][j]=U[i][j]-((L[i][k]*U[k][j])/L[i][i]);
        }
      }
    }
  }
}

template <typename T>
Matrix<T> lu_inverse(Matrix<T>& A){
  assert(A.get_dim1()==A.get_dim2());
  int dim=A.get_dim1();
  
  Matrix<T> L(dim, dim);
  Matrix<T> U(dim, dim);
  Matrix<T> Z(dim, dim);
  Matrix<T> inverse(dim, dim);
  Matrix<T> I(dim, dim);
  for(int i=0; i<dim; ++i){
    I[i][i]=1;
  }
  
  LU_decomposition(A, L, U);
  
  for(int i=0; i<dim; ++i){
    for(int j=0; j<dim; ++j){
      T sum=0;
      for(int k=0; k<dim; ++k){
        if(k!=j) sum+=L[j][k]*Z[k][i];
      }
      T result=I[j][i]-sum;
      result=result/L[j][j];
      Z[j][i]=result;
    }
  }
  
  for(int i=0; i<dim; ++i){
    for(int j=dim-1; j>=0; --j){
      T sum=0;
      for(int k=0; k<dim; ++k){
        if(k!=j) sum+=U[j][k]*inverse[k][i];
      }
      T result=Z[j][i]-sum;
      result=result/U[j][j];
      inverse[j][i]=result;
    }
  }
  
  return inverse;
}

#endif
