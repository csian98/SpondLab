#define _CRT_NO_SECURE_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DATA_SIZE 5
#define BUFFER_SIZE 128

static char* data_path="/Users/csian/Desktop/CP/Photoelectron_Spectroscopy/XPS_Analysis/Generator/tmp_test/spc";
static char* graph_path="/Users/csian/Desktop/CP/Photoelectron_Spectroscopy/XPS_Analysis/Generator/tmp_test/.codes/tmp_graph.R";
static char* tmp_path="/Users/csian/Desktop/CP/Photoelectron_Spectroscopy/XPS_Analysis/Generator/tmp_test/tmp/tmp_";
static char* path_r="/Users/csian/Desktop/CP/Photoelectron_Spectroscopy/XPS_Analysis/Generator/tmp_test/tmp/tmp_sam_peak.txt";
static char* path_w="/Users/csian/Desktop/CP/Photoelectron_Spectroscopy/XPS_Analysis/Generator/tmp_test/tmp/tmp_peak.txt";

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

void sessa_data(FILE* fp, char* eq, float conta, char* down_name){
  fprintf(fp, "SAMPLE SET MATERIAL %s LAYER 0 \n", eq); fflush(fp);
  if(conta!=0){
    fprintf(fp, "SAMPLE ADD LAYER /C/ THICKNESS %.2f ABOVE 0 \n", conta); fflush(fp);
  }
  fprintf(fp, "PROJECT SAVE OUTPUT \"%s\" \n", tmp_path);
  fprintf(fp, "MODEL SIMULATE \n"); fflush(fp);
  
  char* ptr=strchr(eq, '/');
  while(ptr!=NULL){
    *ptr='_';
    ptr=strchr(ptr+1, '/');
  }
  sprintf(down_name, "%s/%s[%.2f]_", data_path, eq, conta);
  strcat(down_name, "reg1.spc");
  
  fprintf(fp, "MODEL SAVE SPECTRA \"%s/%s[%.2f]_\" \n", data_path, eq, conta); fflush(fp);
  sprintf(eq, "%s[%.2f]_", eq, conta);
}

void read_sam(void){
  char* buf=calloc(BUFFER_SIZE, sizeof(char));
  FILE* fp_w;
  FILE* fp_r;
  int i=0, count;
  float peak;
  char peak_name[32];
  
  if((fp_r=fopen(path_r, "r"))==NULL | (fp_w=fopen(path_w, "w"))==NULL){
    perror("fopen");
    exit(1);
  }
  
  fgets(buf, BUFFER_SIZE, fp_r);
  fscanf(fp_r, "%s %s %s  : %d", buf, buf, buf, &count);
  fgets(buf, BUFFER_SIZE, fp_r);
  fgets(buf, BUFFER_SIZE, fp_r);
  fgets(buf, BUFFER_SIZE, fp_r);
  
  while(i<count){
    fscanf(fp_r, "%d %s %s %f", &i, peak_name, buf, &peak);
    strcat(peak_name, buf);
    fgets(buf, BUFFER_SIZE, fp_r);
    fprintf(fp_w, "%s %f\n", peak_name, peak);
  }
  
  fclose(fp_r); fclose(fp_w);
  free(buf);
}

/**
 * @author CSIAN
 * @brief quick data & graph generator
 * @param ./quick_data [eq] [conta]
 *  eq: composition & composed ratio
 *  conta: C5O1 contamination depth[A; 10^-10m]
 * @details
 *  by using sessa make spc data & drawing graph
 *  data:  tmp_test/spc/_eq_[conta]_reg1.spc
 *  graph: tmp_test/graph/_eq_[conta]_.png
 * @date 2021-11-14
 * @version 0.0.1
 */

int main(int argc, char* argv[]){
  char* eq=(char*)calloc(BUFFER_SIZE, sizeof(char));
  char* down_name=(char*)calloc(BUFFER_SIZE, sizeof(char));
  float conta;
  FILE* fp;
  pid_t pid;
  
  if(argc!=3){
    printf("./quick_data [equation] [contamination_depth]\n");
    exit(0);
  }
  strcpy(eq, argv[1]);
  //eq=argv[1];
  conta=atof(argv[2]);
  
  sessa_connect(&fp);
  sessa_data(fp, eq, conta, down_name);
  sessa_disconnect(fp);
  
  read_sam();
  
  while(access(down_name, F_OK)){}
  
  if((pid=fork())==-1){
    perror("fork");
    exit(1);
  }
  
  if(pid==0){
    // Rscript drawing graph
    execlp("Rscript", "Rscript", graph_path, eq, (char*)NULL);
    perror("execl");
    exit(1);
  }
  
  free(eq); free(down_name);
  
  wait(NULL);
  return 0;
}
