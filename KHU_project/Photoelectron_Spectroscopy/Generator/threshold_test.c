#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define XPS_SIZE  2048
#define BUFFER_SIZE 64
#define LAG 15
#define THRESHOLD 20
#define INFLUENCE 0.2

void read_raw(FILE* fp_r, double* index, double* raw_data){
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

// Z-score functions
double mean(double* data, int len){
  float sum=0.0, mean=0.0;
  
  int i;
  for(i=0; i<len; i++){
    sum+=data[i];
  }
  mean=sum/len;
  return mean;
}

double stddev(double* data, int len){
  double the_mean=mean(data, len);
  double standardDeviation=0.0;
  
  int i;
  for(i=0; i<len; i++){
    standardDeviation+=pow(data[i]-the_mean, 2);
  }
  return sqrt(standardDeviation/len);
}

void thresholding(double* raw_data, int* peak_point, int lag, double threshold, double influence){
  memset(peak_point, 0x00, sizeof(int)*XPS_SIZE);
  double filteredY[XPS_SIZE];
  double avgFilter[XPS_SIZE];
  double stdFilter[XPS_SIZE];
  memcpy(filteredY, raw_data, sizeof(float)*XPS_SIZE);
  int i;
  
  avgFilter[lag-1]=mean(raw_data, lag);
  stdFilter[lag-1]=stddev(raw_data, lag);
  
  for(i=lag; i<XPS_SIZE; i++){
    if(fabs(raw_data[i]-avgFilter[i-1])>threshold*stdFilter[i-1]){
      if(raw_data[i]>avgFilter[i-1]){
        peak_point[i]=1;
      }else{
        peak_point[i]=-1;
      }
      filteredY[i]=influence*raw_data[i]+(1-influence)*filteredY[i-1];
    }else{
      peak_point[i]=0;
    }
    avgFilter[i]=mean(filteredY+i-lag, lag);
    stdFilter[i]=stddev(filteredY+i-lag, lag);
  }
}

// test_main
int main(void){
  FILE* fp_r;
  char* path="/Users/csian/Desktop/CP/Photoelectron_Spectroscopy/XPS_Analysis/Generator/.tmp_data/000001_reg1.spc";
  int i, lag;
  //
  double threshold, influence;
  lag=LAG;
  threshold=THRESHOLD;
  influence=INFLUENCE;
  //
  if((fp_r=fopen(path, "r"))==NULL){
    perror("fopen");
    exit(1);
  }
  double* index=(double*)calloc(XPS_SIZE, sizeof(double));
  double* raw_data=(double*)calloc(XPS_SIZE, sizeof(double));
  int* peak_point=(int*)calloc(XPS_SIZE, sizeof(int));
  memset(index, 0x00, sizeof(double)*XPS_SIZE);
  memset(raw_data, 0x00, sizeof(double)*XPS_SIZE);
  
  read_raw(fp_r, index, raw_data);
  thresholding(raw_data, peak_point, lag, threshold, influence);
  
  for(i=0; i<XPS_SIZE; i++){
    if(peak_point[i]==1){
      printf("%le\n", index[i]);
    }else if(peak_point[i]==-1){
      printf("there is -1\n");
    }
  }
  
  fclose(fp_r);
  free(index); free(raw_data);
}
