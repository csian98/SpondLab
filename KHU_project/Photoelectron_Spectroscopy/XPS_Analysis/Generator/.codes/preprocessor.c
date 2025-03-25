#define _CRT_NO_SECURE_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/stat.h>
#include <time.h>
#include <limits.h>
#include <dirent.h>

#define DATA_SIZE 100000
#define DIR_MAX 1000
#define BUFFER_SIZE 128
#define XPS_SIZE  2048
#define _SIGUSR1 10
#define _SIGUSR2 12

static char peak_path[PATH_MAX];

static int state;
void sigusr_rec1(int sig_num){
  state=0;
}
void sigusr_rec2(int sig_num){
  state=-1;
}

void add_info(FILE* fp_w, char* eq, float conta){
  fprintf(fp_w, "# %s [%.2f]\n", eq, conta);
}

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

void normalizer(double* raw_data){
  register double max_value=0, min_value=raw_data[0];
  register int i;
  for(i=0; i<XPS_SIZE; i++){
    if(raw_data[i]>max_value) max_value=raw_data[i];
    if(raw_data[i]<min_value) min_value=raw_data[i];
  }
  for(i=0; i<XPS_SIZE; i++){
    raw_data[i]=(raw_data[i]-min_value)/(max_value-min_value);
  }
}

void write_end(FILE* fp_w, double* index, double* raw_data){
  register int i;
  for(i=0; i<XPS_SIZE; i++){
    fprintf(fp_w, "%le %le\n", index[i], raw_data[i]);
  }
}

void clear_dir(void){
  DIR* tmp_dir;
  struct dirent* dir_ent;
  char* buf=(char*)calloc(BUFFER_SIZE, sizeof(char));
  if((tmp_dir=opendir(peak_path))==NULL){
    perror("opendir");
    exit(1);
  }
  while((dir_ent=readdir(tmp_dir))!=NULL){
    if(strcmp(dir_ent->d_name, ".")==0 || strcmp(dir_ent->d_name, "..")==0){
      continue;
    }
    sprintf(buf, "%s/%s", peak_path, dir_ent->d_name);
    remove(buf);
  }
  closedir(tmp_dir);
}

/**
 * @file preprocessor.c
 * @author CSIAN
 * @version 2.0.1
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
  FILE* fp_w;
  FILE* fp_r;
  double* index=(double*)calloc(XPS_SIZE, sizeof(double));
  double* raw_data=(double*)calloc(XPS_SIZE, sizeof(double));
  char* eq=(char*)calloc(BUFFER_SIZE, sizeof(char));
  float conta=0.0f;
  int file_no, dir_no=0;
  pid_t ppid=getppid();
  char* buf=(char*)calloc(BUFFER_SIZE, sizeof(char));
  char* path_tmp=(char*)calloc(PATH_MAX, sizeof(char));
  memset(eq, 0x00, BUFFER_SIZE*sizeof(char));
  memset(buf, 0x00, BUFFER_SIZE*sizeof(char));
  memset(path_tmp, 0x00, PATH_MAX*sizeof(char));
  
  if(!getcwd(path_tmp, PATH_MAX*sizeof(char))){
    perror("getcwd");
    exit(1);
  }
  sprintf(peak_path, "%s/.tmp_data/tmp", path_tmp);
  
  printf("PREPROCESSOR IS READY\n");
  
  state=1;
  while(1){
    while(state){
      if(state==-1){
        free(eq); free(raw_data); free(index);
        free(buf); free(path_tmp);
        printf("PREPROCESSOR TERMINATE\n");
        exit(0);
      }
    }
    state=1;
    
    read(STDIN_FILENO, eq, BUFFER_SIZE);
    read(STDIN_FILENO, &conta, sizeof(float));
    read(STDIN_FILENO, &file_no, sizeof(int));
    
    if(file_no%DIR_MAX==1){
      dir_no++;
      sprintf(path_tmp, "data_set/dir_%d", dir_no);
      if(access(path_tmp, F_OK)!=0){
        if((mkdir(path_tmp, 0755))==-1){
          perror("mkdir");
          exit(1);
        }
      }
    }
    
    sprintf(path_tmp, ".tmp_data/raw_data/dir_%d", dir_no);
    sprintf(buf, "/%06d_reg1.spc", file_no);
    strcat(path_tmp, buf);
    if((fp_r=fopen(path_tmp, "r"))==NULL){
      perror("fopen_r");
      exit(1);
    }
    
    sprintf(path_tmp, "data_set/dir_%d", dir_no);
    sprintf(buf, "/XPS_%06d", file_no);
    strcat(path_tmp, buf);
    if((fp_w=fopen(path_tmp, "w"))==NULL){
      perror("fopen_w");
      exit(1);
    }
    
    sleep(1);
    read_raw(fp_r, index, raw_data);
    //trans_inten(index, raw_data, peak_point);
    normalizer(raw_data);
    add_info(fp_w, eq, conta);
    write_end(fp_w, index, raw_data);
    clear_dir();
    
    fclose(fp_r); fclose(fp_w);
    kill(ppid, _SIGUSR1);
  }
}
