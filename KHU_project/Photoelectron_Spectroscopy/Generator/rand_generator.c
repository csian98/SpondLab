#define _CRT_NO_SECURE_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define DATA_SIZE 1000
#define BUFFER_SIZE 64
#define ATOM_NUM  81

char* atoms[]={"Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi"};

void rand_generator(char* eq, float* conta){
  int i, j, num;
  num=rand()%5+1;
  
  int atoms_num[num];
  int atoms_ratio[num];
  char buf[BUFFER_SIZE];
  
  for(i=0; i<num; i++){
    atoms_num[i]=rand()%ATOM_NUM;
    for(j=0; j<i; j++){
      if(atoms_num[i]==atoms_num[j]){
        i--;
        break;
      }
    }
  }
  
  *conta=(float)(rand()%4001)/100;
  
  int sum=100;
  for(i=0; i<num-1; i++){
    atoms_ratio[i]=(rand()%(sum-(num-i-1)))+1;
    sum-=atoms_ratio[i];
  }
  atoms_ratio[num-1]=sum;
  
  strcat(eq, "/\0");
  for(i=0; i<num; i++){
    strcat(eq, atoms[atoms_num[i]]);
    sprintf(buf, "%d", atoms_ratio[i]);
    strcat(eq, buf);
    strcat(eq, "/\0");
  }
}
int main(void){
  srand(time(NULL));
  char* eq=(char*)calloc(30, sizeof(char));
  float* conta=(float*)calloc(1, sizeof(float));
  int i=0;
  
  for(i=0; i<DATA_SIZE; i++){
    memset(eq, 0x00, sizeof(char)*30);
    memset(conta, 0x00, sizeof(float));
    rand_generator(eq, conta);
    printf("%s[%.2f]\n", eq, *conta);
  }
  
  free(eq);
  free(conta);
  return 0;
}
