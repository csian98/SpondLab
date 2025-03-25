#include "matrix.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define FILTER_SIZE 12
#define CSV_SIZE 351

Vector<double> read_filter(std::string file_name){
  std::ifstream fp;
  fp.open(file_name);
  if(!fp.is_open()){
    std::cout<<"CAN'T FIND FILE(read_filter)"<<std::endl;
    exit(1);
  }
  char buffer[100];
  Vector<double> filter(FILTER_SIZE);
  for(int i=0; i<FILTER_SIZE; ++i){
    fp.getline(buffer, sizeof(buffer));
    filter[i]=std::stod(buffer);
  }
  fp.close();
  return filter;
}

Vector<double> read_CSV(std::string file_name){
  std::ifstream fp;
  fp.open(file_name);
  if(!fp.is_open()){
    std::cout<<"CAN'T FIND FILE(read_CSV)"<<std::endl;
    exit(1);
  }
  char buffer[100];
  Vector<double> CSV_count(CSV_SIZE);
  for(int i=0; i<CSV_SIZE; ++i){
    fp.getline(buffer, sizeof(buffer));
    CSV_count[i]=std::stod(buffer);
  }
  fp.close();
  return CSV_count;
}

void write_vector(std::string file_name, Vector<double> vec){
  std::ofstream fp;
  fp.open(file_name);
  if(!fp.is_open()){
    std::cout<<"CAN'T OPEN FILE(write_vector)"<<std::endl;
    exit(1);
  }
  for(int i=0; i<vec.get_dim(); ++i){
    fp<<vec[i]<<'\n';
  }
  fp.close();
}
