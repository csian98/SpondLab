#define _CRT_NO_SECURE_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <limits.h>

#define BUFFER_SIZE 64
#define XPS_SIZE  2048

void raw_reader(FILE* fp_r, double* index, double* raw_data){
  char* buf=(char*)calloc(BUFFER_SIZE, sizeof(char));
  int i=0;
  fseek(fp_r, 0, SEEK_SET);
  fgets(buf, BUFFER_SIZE, fp_r);
  while(!feof(fp_r)){
    fscanf(fp_r, "%le %le", &index[i], &raw_data[i]);
    i++;
  }
  free(buf);
}

int main(void){
  FILE* fp_r;
  char* path="/Users/csian/Desktop/CP/Photoelectron_Spectroscopy/XPS_Analysis/Generator/.tmp_data/000002_reg1.spc";
  int i;
  if((fp_r=fopen(path, "r"))==NULL){
    perror("fopen");
    exit(1);
  }
  double* index=(double*)calloc(XPS_SIZE, sizeof(double));
  double* raw_data=(double*)calloc(XPS_SIZE, sizeof(double));
  memset(index, 0x00, sizeof(double)*XPS_SIZE);
  memset(raw_data, 0x00, sizeof(double)*XPS_SIZE);
  
  raw_reader(fp_r, index, raw_data);
  
  for(i=0; i<XPS_SIZE; i++){
    printf("%le %le\n", index[i], raw_data[i]);
  }
  
  fclose(fp_r);
  free(index); free(raw_data);
}
