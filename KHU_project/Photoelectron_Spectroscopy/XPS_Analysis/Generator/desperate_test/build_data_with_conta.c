#define _CRT_NO_SECURE_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 128

char* path="/Users/csian/Desktop/CP/Photoelectron_Spectroscopy/XPS_Analysis/Generator/desperate_test/test_raw";

void sessa_connect(FILE** fp){
  if((*fp=popen("/Users/csian/Desktop/CP/Photoelectron_Spectroscopy/XPS_Analysis/Generator/.codes/.SESSA/SESSA.MAC_v2.2/Sessa\\ v2.2.app/Contents/MacOS/SESSA\\ v2.2 -c", "w"))==NULL){
    perror("popen error"); exit(1);
  }
  fprintf(*fp, "\\PROJECT RESET \n"); fflush(*fp);
  fprintf(*fp, "\\SOURCE SET ALKA \n"); fflush(*fp);
  fprintf(*fp, "\\SPECTROMETER SET RANGE \"400:1486\" REGION 0 \n");
  fprintf(*fp, "\\PREFERENCES SET DENSITY_SCALE MASS \n"); fflush(*fp);
  fprintf(*fp, "\\PREFERENCES SET OUTPUT SAMPLE \"false\" \n"); fflush(*fp);
}

void sessa_disconnect(FILE* fp){
  fprintf(fp, "\\ \n"); fflush(fp);
  fprintf(fp, "quit  \n"); fflush(fp);
  pclose(fp);
}

void sessa_data(FILE* fp, char* eq, int conta){
  char* buf=(char*)calloc(BUFFER_SIZE, sizeof(char));
  sprintf(buf, "%s/%s_%d_", path, eq, conta);
  fprintf(fp, "\\SAMPLE RESET \n"); fflush(fp);
  fprintf(fp, "\\SAMPLE SET MATERIAL %s LAYER 0 \n", eq); fflush(fp);
  if(conta!=0){
    fprintf(fp, "\\SAMPLE ADD LAYER /C5/O/ THICKNESS %d ABOVE 0 \n", conta); fflush(fp);
    fprintf(fp, "\\SAMPLE SET DENSITY 1.56 LAYER 1 \n"); fflush(fp);
  }
  fprintf(fp, "\\MODEL SIMULATE \n"); fflush(fp);
  fprintf(fp, "\\MODEL SAVE SPECTRA \"%s\" \n", buf); fflush(fp);
  free(buf);
}

// ./build_data_with_conta <SUBSTRACT>
int main(int argc, char** argv){
  if(argc!=2){
    perror("./build_data_with_conta <SUBSTRACT>");
    exit(1);
  }
  FILE* fp;
  int conta[]={40, 35, 30, 25, 20, 15, 10};
  
  sessa_connect(&fp);
  for(int i=0; i<7; ++i){
    sessa_data(fp, argv[1], conta[i]);
    sleep(1);
  }
  sessa_data(fp, argv[1], 0);
  sessa_disconnect(fp);
}
