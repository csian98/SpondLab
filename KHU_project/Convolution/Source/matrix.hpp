#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <memory>
#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <cstdlib>

// hpp(declare)
struct LinearAlgebra{
  virtual void print(void) const=0;
};

template <typename T=double>
class Vector: public LinearAlgebra{
public:
  template <typename N>
  friend class Matrix;
  
  Vector(int);
  
  Vector(std::initializer_list<T>);
  
  Vector(const Vector&);
  
  Vector& operator=(const Vector&);
  
  virtual ~Vector(void);
  
  double operator*(const Vector&);
  
  Vector operator*(const double&);
  
  Vector operator+(const Vector&);
  
  T& operator[](int);
  
  void print(void) const override;
  
  int get_dim(void) const;
  
  void replace(int);
private:
  T* vector;
  int dim;
};

template <typename T=double>
class Matrix: public LinearAlgebra{
public:
  Matrix(int, int);
  
  Matrix(std::initializer_list<std::initializer_list<T>>);
  
  Matrix(const Matrix&);
  
  Matrix& operator=(const Matrix&);
  
  virtual ~Matrix(void);
  
  Vector<T> operator*(Vector<T>&);
  
  Matrix operator*(Matrix&);
  
  Matrix operator/(const T);
  
  Vector<T>& operator[](int);
  
  Matrix transpose(void);
  
  void print(void) const override;
  
  int get_dim1(void) const;
  
  int get_dim2(void) const;
  
  bool is_square(void) const;
  
  bool is_identity(void) const;
private:
  Vector<T>* matrix;
  int dim_1, dim_2;
};

// cpp(definition)
template <typename T>
Vector<T>::Vector(int dim): dim(dim) , vector(new T[dim]) {}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> li): dim(li.size()){
  vector=new T[dim];
  std::copy(std::begin(li), std::end(li), vector);
}

template <typename T>
Vector<T>::Vector(const Vector& other): dim(other.dim) , vector(new T[other.dim]){
  for(int i=0; i<dim; ++i){
    vector[i]=other.vector[i];
  }
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other){
  assert(dim==other.dim);
  if(this!=&other){
    for(int i=0; i<dim; ++i){
      vector[i]=other.vector[i];
    }
  }
  return *this;
}

template <typename T>
Vector<T>::~Vector(void){
  delete[] vector;
}

template <typename T>
double Vector<T>::operator*(const Vector& other){
  assert(dim==other.dim);
  double sum=0;
  for(int i=0; i<dim; ++i){
    sum+=vector[i]*other.vector[i];
  }
  return sum;
}

template <typename T>
Vector<T> Vector<T>::operator*(const double& other){
  Vector<T> tmp(dim);
  
  for(int i=0; i<dim; ++i){
    tmp.vector[i]=vector[i]*other;
  }
  return tmp;
}

template <typename T>
Vector<T> Vector<T>::operator+(const Vector& other){
  assert(dim==other.dim);
  Vector<T> tmp(dim);
  
  for(int i=0; i<dim; ++i){
    tmp.vector[i]=vector[i]+other.vector[i];
  }
  return tmp;
}

template <typename T>
T& Vector<T>::operator[](int ind){
  return vector[ind];
}

template <typename T>
void Vector<T>::print(void) const {
  for(int i=0; i<dim-1; ++i){
    std::cout<<vector[i]<<", ";
  }
  std::cout<<vector[dim-1]<<std::endl;
}

template <typename T>
int Vector<T>::get_dim(void) const {
  return dim;
}

template <typename T>
void Vector<T>::replace(int dim_2){
  delete[] vector;
  
  dim=dim_2;
  vector=new T[dim_2]{};
}

template <typename T>
Matrix<T>::Matrix(const int dim_1, const int dim_2): dim_1(dim_1), dim_2(dim_2), matrix(new Vector<T>[dim_1]{}) {
  for(int i=0; i<dim_1; ++i){
    matrix[i].replace(dim_2);
  }
}

template <typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> li): dim_1(li.size()), dim_2(li.begin()->size()), matrix(new Vector<T>[li.size()]{}){
  for(int i=0; i<dim_1; ++i){
    matrix[i].replace(dim_2);
  }
  int i=0;
  for(auto iter=li.begin(); iter<li.end(); iter++, i++){
    assert(dim_2==iter->size());
    std::copy(iter->begin(), iter->end(), matrix[i].vector);
  }
}

template <typename T>
Matrix<T>::Matrix(const Matrix& other): dim_1(other.dim_1), dim_2(other.dim_2), matrix(new Vector<T>[other.dim_1]{} ){
  for(int i=0; i<dim_1; ++i){
    matrix[i].replace(dim_2);
  }
  for(int i=0; i<dim_1; ++i){
    for(int j=0; j<dim_2; ++j){
      matrix[i][j]=other.matrix[i][j];
    }
  }
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix& other){
  assert(dim_1==other.dim_1 && dim_2==other.dim_2);
  for(int i=0; i<dim_1; ++i){
    for(int j=0; j<dim_2; ++j){
      matrix[i][j]=other.matrix[i][j];
    }
  }
  return *this;
}

template <typename T>
Matrix<T>::~Matrix(void){
  delete[] matrix;
}

template <typename T>
Vector<T> Matrix<T>::operator*(Vector<T>& other){
  assert(dim_2==other.get_dim());
  Vector<T> tmp(dim_1);
  for(int i=0; i<dim_1; ++i){
    tmp[i]=matrix[i]*other;
  }
  return tmp;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(Matrix& other){
  assert(dim_2==other.dim_1);
  Matrix<T> tmp(dim_1, other.dim_2);
  Matrix<T> other_trans=other.transpose();
  
  for(int i=0; i<dim_1; ++i){
    for(int j=0; j<other.dim_2; ++j){
      tmp[i][j]=matrix[i]*other_trans[j];
    }
  }
  return tmp;
}

template <typename T>
Matrix<T> Matrix<T>::operator/(const T other){
  Matrix<T> tmp(dim_1, dim_2);
  for(int i=0; i<dim_1; ++i){
    for(int j=0; j<dim_2; ++j){
      tmp[i][j]=matrix[i][j]/other;
    }
  }
  return tmp;
}

template <typename T>
Vector<T>& Matrix<T>::operator[](int index){
  return matrix[index];
}

template <typename T>
Matrix<T> Matrix<T>::transpose(void){
  Matrix<T> tmp(dim_2, dim_1);
  for(int i=0; i<dim_1; ++i){
    for(int j=0; j<dim_2; ++j){
      tmp[j][i]=matrix[i][j];
    }
  }
  return tmp;
}

template <typename T>
void Matrix<T>::print(void) const {
  for(int i=0; i<dim_1; ++i){
    for(int j=0; j<dim_2-1; ++j){
      std::cout<<matrix[i][j]<<", ";
    }
    std::cout<<matrix[i][dim_2-1]<<std::endl;
  }
  std::cout<<std::endl;
}

template <typename T>
int Matrix<T>::get_dim1(void) const {
  return dim_1;
}

template <typename T>
int Matrix<T>::get_dim2(void) const {
  return dim_2;
}

template <typename T>
bool Matrix<T>::is_square(void) const {
  return dim_1==dim_2;
}

template <typename T>
bool Matrix<T>::is_identity(void) const {
  for(int i=0; i<dim_1; ++i){
    for(int j=0; j<dim_2; ++j){
      if(matrix[i][j]==1 && i==j) continue;
      else if(matrix[i][j]==0 && i!=j) continue;
      else return false;
    }
  }
  return true;
}

#endif
