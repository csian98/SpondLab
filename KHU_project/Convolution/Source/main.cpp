#include <chrono>

#include "matrix.hpp"
#include "inverse.hpp"
#include "lu_inverse.hpp"
#include "plu_inverse.hpp"
#include "read_data.hpp"

template <typename T=double>
Matrix<T> BuildFilter(int m_size, int ft_size){
  assert(m_size>=ft_size);
  Matrix<T> tmp(m_size, m_size);
  for(int i=0; i<m_size; ++i){
    for(int j=0; j<ft_size; j++){
      if(i-(ft_size/2)+j<0 || i-(ft_size/2)+j>=m_size) continue;
      tmp[i][i-(ft_size/2)+j]=1;
      //if(i+j<0 || i+j>=m_size) continue;
      //tmp[i][i+j]=1;
    }
  }
  return tmp;
}

template <typename T=double>
Matrix<T> BuildFilter(int m_size, Vector<T>& vec){
  assert(m_size>=vec.get_dim());
  int ft_size=vec.get_dim();
  Matrix<T> tmp(m_size, m_size);
  for(int i=0; i<m_size; ++i){
    for(int j=0; j<ft_size; ++j){
      if(i-(ft_size/2)+j<0 || i-(ft_size/2)+j>=m_size) continue;
      tmp[i][i-(ft_size/2)+j]=vec[j];
    }
  }
  return tmp;
}

int main(void){
  using namespace std::chrono;
  
  Vector<double> filter(FILTER_SIZE);
  Vector<double> before(CSV_SIZE);
  Vector<double> after(CSV_SIZE);
  int dim=CSV_SIZE;
  
  filter=read_filter("./filter.txt");
  before=read_CSV("./data.txt");
  
  Matrix<double> toeplitz(dim, dim);
  toeplitz=BuildFilter(dim, filter);
  
  Matrix<double> inverse(dim, dim);
  inverse=plu_inverse(toeplitz);
  
  after=inverse*before;
  write_vector("after.txt", after);
  
  return 0;
}
