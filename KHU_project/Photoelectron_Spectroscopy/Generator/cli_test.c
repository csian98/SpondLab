#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(void){
  double x[100], y[100];
  int k;
  char c[100];
  FILE* fp;
  
  char* path="/Users/csian/Desktop/CP/Photoelectron_Spectroscopy";
  
  if((fp=popen("../SESSA.MAC_v2.2/Sessa\\ v2.2.app/Contents/MacOS/SESSA\\ v2.2 -c", "w"))==NULL){
    perror("popen");exit(1);
  }
  fprintf(fp, "PROJECT RESET\n"); fflush(fp);
  fprintf(fp, "SAMPLE SET MATERIAL /Si/O2/ LAYER 1\n"); fflush(fp);
  fprintf(fp, "MODEL SIMULATE\n"); fflush(fp);
  fprintf(fp, "SAMPLE PEAK PLOT PART_INT PEAK 1\n"); fflush(fp);
  fprintf(fp, "PLOT SAVE DATA \"%s/Generator/Saved//Si2s\"\n", path); fflush(fp);
  fprintf(fp, "SAMPLE PEAK PLOT PART_INT PEAK 2\n"); fflush(fp);
  fprintf(fp, "PLOT SAVE DATA \"%s/Generator/Saved//Si2p1\"\n", path); fflush(fp);
  fprintf(fp, "SAMPLE PEAK PLOT PART_INT PEAK 3\n"); fflush(fp);
  fprintf(fp, "PLOT SAVE DATA \"%s/Generator/Saved//Si2p3\"\n", path); fflush(fp);
  fprintf(fp, "SAMPLE PEAK PLOT PART_INT PEAK 4\n"); fflush(fp);
  fprintf(fp, "PLOT SAVE DATA \"%s/Generator/Saved//O1s\"\n", path); fflush(fp);
  fprintf(fp, "quit\n"); fflush(fp);
  pclose(fp);
  
  fp=fopen("Saved//Si2s", "r");
  if(fp==NULL){
    printf("Error:can't open file.\n");
  }else{
    while(fgets(c, 100, fp)!=NULL){
      if(c[0]=='#')continue;
      fscanf(fp, "%le %le", &x[k], &y[k]);
      printf("%le %le\n", x[k], y[k]);
      k++;
    }
    fclose(fp);
  }
}
