#define _CRT_NO_SECURE_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define DATA_SIZE 1000
#define BUFFER_SIZE 64
#define ATOM_NUM  81
#define PATH_MAX 1024

void add_info(FILE* fp_w, char* eq, float* conta){
  // "#[eq]\t[C_conta]
  char info[BUFFER_SIZE];
  char conta_str[16];
  
  sprintf(conta_str, "\t%s\n", (char*)conta);
  strcat(info, eq);
  strcat(info, conta_str);
  fwrite(info, 1, sizeof(info), fp_w);
}

int preprocess(int file_no, char* eq, float* conta){
  FILE* fp_w;
  char path_w[BUFFER_SIZE]="./Data_set/";
  char file_name_w[16];
  sprintf(file_name_w, "XPS_%06d", file_no);
  strcat(path_w, file_name_w);
  
  if((fp_w=fopen(path_w, "w"))==NULL){
    perror("fopen");
    exit(1);
  }
  
  // add_info()
  // find_peak()
  // trans_()
  
  fclose(fp_w);
  
  return 0;
}

int main(void){
  //write test
  char* eq="/Si33/O67/";
  float* conta=(float*)calloc(1, sizeof(float));
  *conta=15.20f;
  int file_no=50;
  
  preprocess(file_no, eq, conta);
  return 0;
}
