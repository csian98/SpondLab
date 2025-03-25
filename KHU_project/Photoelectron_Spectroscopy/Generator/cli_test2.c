#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void){
  double x, y;
  int k;
  char c[100];
  FILE* fp;
  char* path="/Users/csian/Desktop/CP/Photoelectron_Spectroscopy";
  
  if((fp=popen("../SESSA.MAC_v2.2/Sessa\\ v2.2.app/Contents/MacOS/SESSA\\ v2.2 -c", "w"))==NULL){
    perror("popen");exit(1);
  }
  
  fprintf(fp, "PROJECT RESET\n"); fflush(fp);
  fprintf(fp, "SAMPLE SET MATERIAL /Si/O2/ Layer 1\n"); fflush(fp);
  fprintf(fp, "MODEL SET NCOL 10 REGION 1\n"); fflush(fp);
  fprintf(fp, "MODEL SIMULATE\n"); fflush(fp);
  fprintf(fp, "PROJECT SAVE OUTPUT \"%s/Generator/Saved2/test\"\n", path); fflush(fp);
  fprintf(fp, "SAMPLE PEAK PLOT PART_INT PEAK 1\n"); fflush(fp);
  fprintf(fp, "PLOT SAVE DATA \"%s/Generator/saved2/Si2p.cn\"\n", path); fflush(fp);
  fprintf(fp, "quit\n"); fflush(fp);
  pclose(fp);
  
  fp=fopen("Saved2/Si2p.cn", "r");
  if(fp==NULL){
    printf("First partial intensities: \n");
    while(fgets(c, 100, fp)!=NULL){
      if(c[0]=='#')continue;
      fscanf(fp, "%le %le", &x, &y);
      if(k>1){ printf("%le %le\n", x, y);}
      k++;
    }
  }
}
