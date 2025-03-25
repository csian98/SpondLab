#define _CRT_NO_SECURE_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/stat.h>
#include <time.h>
#include <limits.h>

#define DATA_SIZE 100
#define DIR_MAX 1000
#define BUFFER_SIZE 128
#define ATOM_NUM  80
#define _SIGUSR1 10
#define _SIGUSR2 12

// atoms from [3, 83] except Pm_61
static char* atoms[]={"Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi"};

static char path_cur[PATH_MAX];
static char path[PATH_MAX];
static char peak_path[PATH_MAX];

static int state;

void sigusr_rec1(int sig_num){
  state=0;
}

void sigusr_rec2(int sig_num){
  printf("GENERATOR RELOADING\n");
  state=-1;
}

void rand_generator(char** eq, float* conta){
  int i, j, num, tmp;
  num=rand()%4+2;
  
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
  
  for(i=1; i<num; i++){
    tmp=atoms_ratio[i];
    j=i-1;
    while(j>=0 && atoms_ratio[j]<tmp){
      atoms_ratio[j+1]=atoms_ratio[j];
      j--;
    }
    atoms_ratio[j+1]=tmp;
  }
  
  strcat(*eq, "/");
  for(i=0; i<num; i++){
    strcat(*eq, atoms[atoms_num[i]]);
    sprintf(buf, "%d", atoms_ratio[i]);
    strcat(*eq, buf);
    strcat(*eq, "/");
  }
}

void sessa_connect(FILE** fp){
  if((*fp=popen(".codes/.SESSA/SESSA.MAC_v2.2/Sessa\\ v2.2.app/Contents/MacOS/SESSA\\ v2.2 -c", "w"))==NULL){
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

void sessa_conta_shift(FILE* fp, int peak_num){
  float shift=(float)(rand()%101)/100-0.5;
  if(shift!=0){
    fprintf(fp, "\\SAMPLE PEAK SET CHEMSHIFT VALUE %.2f PEAK %d \n", shift, peak_num); fflush(fp);
  }
}

void sessa_chemi_shift(FILE* fp, int peak_num){
  float shift=(float)(rand()%2001)/200-10;
  if(shift!=0){
    fprintf(fp, "\\SAMPLE PEAK SET CHEMSHIFT VALUE %.2f PEAK %d \n", shift, peak_num); fflush(fp);
  }
}

void shift_sam(FILE* fp){
  char* buf=calloc(BUFFER_SIZE, sizeof(char));
  FILE* fp_r;
  int i=0, count=0;
  float peak;
  char peak_name[32];
  
  sleep(1);
  sprintf(buf, "%s/tmp_sam_peak.txt", peak_path);
  while(1){
    if(access(buf, F_OK)==0){
      break;
    }else{
      continue;
    }
  }
  if((fp_r=fopen(buf, "r"))==NULL){
    perror("fopen_r");
    exit(1);
  }
  fgets(buf, BUFFER_SIZE, fp_r);
  fscanf(fp_r, "%s %s %s  : %d", buf, buf, buf, &count);
  fgets(buf, BUFFER_SIZE, fp_r);
  fgets(buf, BUFFER_SIZE, fp_r);
  fgets(buf, BUFFER_SIZE, fp_r);
  
  while(i<count){
    fscanf(fp_r, "%d %s %s %f", &i, peak_name, buf, &peak);
    fgets(buf, BUFFER_SIZE, fp_r);
    if(strcmp(peak_name, "C")==0 || strcmp(peak_name, "O")){
      sessa_conta_shift(fp, i);
    }else{
      sessa_chemi_shift(fp, i);
    }
  }
  fclose(fp_r);
  free(buf);
}

void sessa_data(FILE* fp, int file_no, char* eq, float conta){
  char* buf=(char*)calloc(BUFFER_SIZE, sizeof(char));
  sprintf(buf, "%s/tmp_", peak_path);
  fprintf(fp, "\\SAMPLE RESET \n"); fflush(fp);
  fprintf(fp, "\\SAMPLE SET MATERIAL %s LAYER 0 \n", eq); fflush(fp);
  if(conta!=0){
    fprintf(fp, "\\SAMPLE ADD LAYER /C5/O/ THICKNESS %.2f ABOVE 0 \n", conta); fflush(fp);
    fprintf(fp, "\\SAMPLE SET DENSITY 1.56 LAYER 1 \n"); fflush(fp);
  }
  fprintf(fp, "\\PROJECT SAVE OUTPUT \"%s\" \n", buf); fflush(fp);
  
  shift_sam(fp);
  
  fprintf(fp, "\\MODEL SIMULATE \n"); fflush(fp);
  fprintf(fp, "\\MODEL SAVE SPECTRA \"%s/%06d_\" \n", path, file_no); fflush(fp);
  free(buf);
}

void write_pid(pid_t pid_p, pid_t pid_m){
  pid_t pid=getpid();
  FILE* fp=fopen("./.tmp_data/pid.txt", "w");
  fprintf(fp, "GENERATOR= %d\nPREPROCESS= %d\nGENERATOR_MASTER= %d", pid, pid_p, pid_m);
  fclose(fp);
}

/**
 * @file generator.c
 * @author CSIAN
 * @version 2.0.2
 * @date
 * 2021-11-22
 * @section LICENSE
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 * 
 * @brief
 * Please refer to README(generator)
 * @param
 * Please refer to README(generator)
 * @details
 * Please refer to README(generator)
 */

int main(void){
  signal(_SIGUSR1, sigusr_rec1);
  signal(_SIGUSR2, sigusr_rec2);
  srand(time(NULL));
  
  FILE* fp, fp_pid;
  char* eq=(char*)calloc(BUFFER_SIZE, sizeof(char));
  char* path_tmp=(char*)calloc(PATH_MAX, sizeof(char));
  char* buf=(char*)calloc(BUFFER_SIZE, sizeof(char));
  int fd[2], file_no, i, dir_no=0;
  float conta=0.0f;
  pid_t pid_m, pid_p;
  
  if(!getcwd(path_cur, sizeof(path_cur))){
    perror("getcwd");
    exit(1);
  }
  sprintf(peak_path, "%s/.tmp_data/tmp", path_cur);
  
  if(pipe(fd)<0){
    perror("pipe");
    exit(1);
  }
  
  if((pid_m=fork())==-1){
    perror("fork_m");
    exit(1);
  }
  
  if(pid_m==0){
    execl(".codes/generator_master", (char*)0);
    perror("execl_generator_master");
    exit(1);
  }
  
  if((pid_p=fork())==-1){
    perror("fork_p");
    exit(1);
  }
  
  if(pid_p==0){
    close(STDIN_FILENO);
    dup(fd[0]); close(fd[0]);
    
    execl(".codes/preprocessor", (char*)0);
    perror("execl_preprocessor");
    exit(1);
  }
  
  printf("ORDER DATA SET %d\n", DATA_SIZE);
  printf("GENERTATOR IS READY\n");
  
  write_pid(pid_p, pid_m);
  
  sleep(5);
  
  kill(pid_m, _SIGUSR2);
  state=0;
  
  for(i=0; i<DATA_SIZE; i++){
    sessa_connect(&fp);
    memset(eq, 0x00, sizeof(char)*BUFFER_SIZE);
    memset(path_tmp, 0x00, sizeof(char)*PATH_MAX);
    file_no=i+1;
    if(file_no%DIR_MAX==1){
      dir_no++;
      sprintf(buf, "%s/.tmp_data/raw_data/dir_%d", path_cur, dir_no);
      strcpy(path, buf);
      if(access(buf, F_OK)!=0){
        if((mkdir(buf, 0755))==-1){
          perror("mkdir");
          exit(1);
        }
      }
    }
    
    rand_generator(&eq, &conta);
    
    while(state){}
    state=1;
    
    sessa_data(fp, file_no, eq, conta);
    
    sprintf(buf, "/%06d_reg1.spc", file_no);
    strcpy(path_tmp, path);
    strcat(path_tmp, buf);
    
    while(1){
      if(access(path_tmp, F_OK)==0){
        break;
      }else{
        if(state==-1){
          break;
        }
        continue;
      }
    }
    if(state==-1){
      state=0; i--;
      kill(pid_m, _SIGUSR1);
      sessa_disconnect(fp);
      continue;
    }
    
    write(fd[1], eq, BUFFER_SIZE*sizeof(char));
    write(fd[1], &conta, sizeof(float));
    write(fd[1], &file_no, sizeof(int));
    sessa_disconnect(fp);
    
    kill(pid_p, _SIGUSR1);
    kill(pid_m, _SIGUSR1);
  }
  while(state){}
  printf("GENERATOR PROCESS END\n");
  sleep(1);
  
  kill(pid_p, _SIGUSR2);
  waitpid(pid_p, &state, 0);
  kill(pid_m, _SIGUSR2);
  waitpid(pid_m, &state, 0);
  
  free(eq); free(path_tmp); free(buf);
  
  if(remove("./.tmp_data/pid.txt")==-1){
    perror("remove pid.txt");
    exit(1);
  }
  printf("GENERTOR TERMINATE\n");
  return 0;
}
