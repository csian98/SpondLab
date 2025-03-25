#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double gaussianRandom(void){
  double v1, v2, s;
  do{
    v1=2*((double)rand()/RAND_MAX)-1;
    v2=2*((double)rand()/RAND_MAX)-1;
    s=v1*v1+v2*v2;
  }while(s>=1 || s==0);
  s= sqrt((-2*log(s))/s);
  
  return v1*s;
}

double gaussian_Random(double average=0, double stdev=1){
  double v1, v2, s, temp;
  do{
    v1=2*((double)rand()/RAND_MAX)-1;
    v2=2*((double)rand()/RAND_MAX)-1;
    s=v1*v1+v2*v2;
  }while(s>=1 || s==0);
  s=sqrt((-2*log(s))/s);
  temp=v1*s;
  temp=(stdev*temp)+average;
  
  return temp;
}

int main(void){
  int i;
  printf("Gaussian[mean=0, std=1]\n");
  for(i=0; i<50; i++){
    printf("%.4f\n", gaussianRandom());
  }
  printf("\nGaussian[mean=10, std=3]\n");
  for(i=0; i<50; i++){
    printf("%.4f\n", gaussian_Random(10, 3));
  }
  
  return 0;
}
