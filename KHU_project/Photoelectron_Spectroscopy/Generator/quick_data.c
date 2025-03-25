#define _CRT_NO_SECURE_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DATA_SIZE 5
#define BUFFER_SIZE 64

char* path=0;

void sessa_connect(FILE** fp){
  if((*fp=popen(".codes/.SESSA/SESSA.MAC_v2.2/Sessa\\ v2.2.app/Contents/MacOS/SESSA\\ v2.2 -c", "w"))==NULL){
    perror("popen error"); exit(1);
  }
  fprintf(*fp, "PROJECT RESET \n"); fflush(*fp);
  fprintf(*fp, "SOURCE SET ALKA \n"); fflush(*fp);
  fprintf(*fp, "SPECTROMETER SET RANGE \"400:1486\" REGION 0 \n"); fflush(*fp);
  fprintf(*fp, "PREFERENCES SET DENSITY_SCALE MASS \n"); fflush(*fp);
  fprintf(*fp, "PREFERENCES SET OUTPUT SAMPLE \"false\" \n"); fflush(*fp);
}

void sessa_disconnect(FILE* fp){
  fprintf(fp, "quit \n"); fflush(fp);
  pclose(fp);
}

void sessa_data(FILE* fp, int file_no, char* eq, float* conta){
  fprintf(fp, "SAMPLE SET MATERIAL %s LAYER 0 \n", eq); fflush(fp);
  fprintf(fp, "SAMPLE ADD LAYER /C/ THICKNESS %.2f ABOVE 0 \n", *conta); fflush(fp);
  fprintf(fp, "MODEL SIMULATE \n"); fflush(fp);
  fprintf(fp, "MODEL SAVE SPECTRA \"%s/%06d_\" \n", path, file_no); fflush(fp);
}

// ./quick_data [eq] [conta_depth]
int main(int argc, char* argv[]){
  char* eq=calloc(BUFFER_SIZE, sizeof(char));
  float conta;
  
  if(argc!=3){
    printf("./quick_data [equation] [contamination_depth]\n");
    exit(0);
  }
  eq=argv[1];
  conta=atof(argv[2]);
  
  printf("%s %f\n", eq, conta);
  return 0;
}
