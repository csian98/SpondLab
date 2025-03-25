#ifndef INVERSE_HPP
#define INVERSE_HPP

#include <iostream>
#include <memory>
#include <cassert>
#include <algorithm>
#include <cstdlib>

#include "matrix.hpp"

// hpp(declare)
template <typename T>
Matrix<T> cofactor(Matrix<T>&, int, int);

template <typename T>
double determinant(Matrix<T>&, int);

template <typename T>
Matrix<T> adjoint(Matrix<T>&);

template <typename T>
Matrix<T> inverrse(Matrix<T>&);

// cpp(definition)
template <typename T>
Matrix<T> cofactor(Matrix<T>& other, int p, int q){
  int dim1=other.get_dim1();
  int dim2=other.get_dim2();
  assert(dim1==dim2);
  Matrix tmp(dim1-1, dim2-1);
  int n=0, m=0;
  for(int i=0; i<dim1; ++i){
    for(int j=0; j<dim2; ++j){
      if(i!=p && j!=q){
        tmp[n][m++]=other[i][j];
        if(m==dim1-1){
          m=0;
          n++;
        }
      }
    }
  }
  return tmp;
}

template <typename T>
double determinant(Matrix<T>& other){
  assert(other.get_dim1()==other.get_dim2());
  if(other.get_dim1()==1){
    return other[0][0];
  }
  
  T result=0;
  int dim=other.get_dim1();
  
  int cz, z_ind=0, max_cz=0;
  for(int i=0; i<dim; ++i){
    cz=0;
    for(int j=0; j<dim; ++j){
      if(other[i][j]==0) cz++;
    }
    if(cz>max_cz){
      max_cz=cz;
      z_ind=i;
    }
  }
  
  Matrix<T> tmp(dim-1, dim-1);
  int sign=z_ind%2? -1: 1;
  for(int i=0; i<dim; ++i){
    if(other[z_ind][i]==0){
      sign*=-1;
      continue;
    }
    tmp=cofactor(other, z_ind, i);
    result+=sign*other[z_ind][i]*determinant(tmp);
    sign*=-1;
  }
  return result;
}

template <typename T>
Matrix<T> adjoint(Matrix<T>& other){
  assert(other.get_dim1()==other.get_dim2());
  int dim=other.get_dim1();
  Matrix<T> tmp(dim, dim);
  if(other.get_dim1()==1){
    tmp[0][0]=1;
    return tmp;
  }
  
  Matrix<T> cof(dim-1, dim-1);
  int sign;
  for(int i=0; i<dim; ++i){
    for(int j=0; j<dim; ++j){
      cof=cofactor(other, i, j);
      sign=((i+j)%2)?-1: 1;
      tmp[i][j]=sign*(determinant(cof));
    }
  }
  std::cout<<std::endl;
  return tmp.transpose();
}

template <typename T>
Matrix<T> inverse(Matrix<T>& other, bool time_check=false){
  assert(other.get_dim1()==other.get_dim2());
  int dim=other.get_dim1();
  
  time_t t1, t2;
  if(time_check){
    time(&t1);
  }
  
  double det=determinant(other);
  Matrix<T> tmp(dim, dim);
  if(det==0){
    std::cout<<"NOT A SIGULAR MATRIX"<<std::endl;
    return tmp;
  }
  tmp=adjoint(other)/det;
  if(time_check){
    time(&t2);
    std::cout<<"COST TIME: "<<t2-t1<<std::endl;
  }
  return tmp;
}

#endif
