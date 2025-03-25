#define _CRT_NO_SECURE_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <limits.h>

#define BUFFER_SIZE 128
#define ATOM_NUM  80
#define XPS_SIZE  2048

// atoms from [3, 83] except Pm_61
static char* atoms[]={"Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi"};

static char path_cur[PATH_MAX];
static char path_read[PATH_MAX];
static char path_write[PATH_MAX];

void sessa_connect(FILE** fp){
  if((*fp=popen("./../.codes/.SESSA/SESSA.MAC_v2.2/Sessa\\ v2.2.app/Contents/MacOS/SESSA\\ v2.2 -c", "w"))==NULL){
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

void build_tmp(FILE* fp, char* atom){
  fprintf(fp, "\\SAMPLE RESET \n"); fflush(fp);
  fprintf(fp, "\\SAMPLE SET MATERIAL %s LAYER 0 \n", atom); fflush(fp);
  fprintf(fp, "\\MODEL SIMULATE \n"); fflush(fp);
  fprintf(fp, "\\MODEL SAVE SPECTRA \"%s/tmp_\" \n", path_cur);
}


double get_max(void){
  FILE* fp_r;
  double xps[XPS_SIZE];
  double tmp;
  register double max_value=0;
  char* buf=(char*)calloc(BUFFER_SIZE, sizeof(char));
  int i=0;
  
  if((fp_r=fopen(path_read, "r"))==NULL){
    perror("fopen_r");
    exit(1);
  }
  
  fseek(fp_r, 0, SEEK_SET);
  fgets(buf, BUFFER_SIZE, fp_r);
  while(!feof(fp_r)){
    fscanf(fp_r, "%le %le", &tmp, &xps[i]);
    ++i;
  }
  free(buf);
  fclose(fp_r);
  
  for(i=0; i<XPS_SIZE; ++i){
    if(xps[i]>max_value) max_value=xps[i];
  }
  return max_value;
}


int main(void){
  FILE *fp, *fp_w;
  double* T=(double*)calloc(ATOM_NUM, sizeof(double));
  
  if(!getcwd(path_cur, sizeof(path_cur))){
    perror("getcwd");
    exit(1);
  }
  sprintf(path_read, "%s/tmp_reg1.spc", path_cur);
  sprintf(path_write, "%s/T.txt", path_cur);
  
  if((fp_w=fopen(path_write, "w"))==NULL){
    perror("fopen_w");
    exit(1);
  }
  
  sessa_connect(&fp);
  
  for(int i=0; i<ATOM_NUM; ++i){
    build_tmp(fp, atoms[i]);
    sleep(3);
    T[i]=get_max();
  }
  
  sessa_disconnect(fp);
  
  // T[6]==1 make Fluorin intensity 1
  double fl=T[6];
  for(int i=0; i<ATOM_NUM; ++i){
    T[i]/=fl;
  }
  
  for(int i=0; i<ATOM_NUM; ++i){
    fprintf(fp_w, "%le\n", T[i]);
  }
  
  printf("Finish Building \"XPS relative intensity\"");
  
  if(remove("tmp_reg1.spc")==-1){
    perror("remove tmp_reg1.spc");
    exit(1);
  }
  free(T); fclose(fp_w);
  return 0;
}
