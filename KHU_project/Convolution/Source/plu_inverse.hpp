#ifndef PLU_INVERSE_HPP
#define PLU_INVERSE_HPP

#include <iostream>
#include <memory>
#include <cassert>
#include <algorithm>
#include <cstdlib>

#include "matrix.hpp"

template <typename T>
bool plu(Matrix<T>& A, Matrix<T>& P, Matrix<T>& L, Matrix<T>& U){
  int dim=A.get_dim1();
  int row_index[dim];
  for(int i=0; i<dim; ++i) row_index[i]=i;
  
  U=A;
  for(int i=0; i<dim; ++i){
    L[i][i]=1;
  }
  
  Vector<T> tmp_vec(dim);
  T tmp;
  
  for(int i=0; i<dim; ++i){
    int pivot_index=-1;
    T maximum=0;
    for(int j=i; j<dim; ++j){
      T value=std::abs(U[j][i]);
      
      if(value>maximum){
        pivot_index=j;
        maximum=value;
      }
    }
    
    T pivot=U[pivot_index][i];
    if(pivot==0.0f){
      return false;
    }
    
    if(pivot_index>i){
      std::swap(row_index[i], row_index[pivot_index]);
      
      tmp_vec=U[i];
      U[i]=U[pivot_index];
      U[pivot_index]=tmp_vec;
      
      for(int k=0; k<i; ++k){
        tmp=L[i][k];
        L[i][k]=L[pivot_index][k];
        L[pivot_index][k]=tmp;
      }
    }
    T invPivot=1/pivot;
    
    for(int j=i+1; j<dim; ++j){
      T scalar=U[j][i]*invPivot;
      U[j][i]=0;
      
      for(int k=i+1; k<dim; ++k){
        U[j][k]-=scalar*U[i][k];
      }
      L[j][i]=scalar;
    }
  }
  for(int i=0; i<dim; ++i){
    P[i][row_index[i]]=1;
  }
  return true;
}

template <typename T>
Matrix<T> plu_inverse(Matrix<T>& A){
  assert(A.get_dim1()==A.get_dim2());
  int dim=A.get_dim1();
  
  Matrix<T> P(dim, dim);
  Matrix<T> L(dim, dim);
  Matrix<T> U(dim, dim);
  Matrix<T> Z(dim, dim);
  Matrix<T> inverse(dim, dim);
  Matrix<T> I(dim, dim);
  for(int i=0; i<dim; ++i){
    I[i][i]=1;
  }
  // plu decomposition
  if(!plu(A, P, L, U)){
    std::cout<<"DETERMINANT IS 0"<<std::endl;
    exit(1);
  }
  
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
  return inverse*P;
}

#endif
