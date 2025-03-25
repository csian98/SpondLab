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

int main(void){
  int pid, state=0, state_end;
  int fd_eq[2], fd_conta[2], fd_file_no[2], fd_state[2], fd_state_end[2];
  
  if(pipe(fd_eq)<0 | pipe(fd_conta)<0 | pipe(fd_file_no) | pipe(fd_state) | pipe(fd_state_end)){
    perror("pipe");
    exit(1);
  }
  
  char* eq=(char*)calloc(BUFFER_SIZE, sizeof(char));
  sprintf(eq, "/Si33/O67/");
  float* conta=(float*)calloc(1, sizeof(float));
  *conta=15.20f;
  int file_no=5;
  
  pid=fork();
  if(pid==-1){
    perror("fork");
    exit(1);
  }
  if(pid>0){
    // for!!
    //rand_generator();
    //sessa();
    while(state){
      read(fd_state[0], &state, sizeof(int));
    }
    state=1;
    
    write(fd_eq[1], eq, sizeof(eq));
    write(fd_conta[1], conta, sizeof(float));
    write(fd_file_no[1], &file_no, sizeof(int));
    state_end=0;
    write(fd_state_end[1], &state_end, sizeof(int));
    // for fin
  }else{
    //while(1)
    state_end=1;
    while(state_end){
      read(fd_state_end[0], &state_end, sizeof(int));
    }
    read(fd_eq[0], eq, BUFFER_SIZE*sizeof(char));
    read(fd_conta[0], conta, sizeof(float));
    read(fd_file_no[0], &file_no, sizeof(int));
    //preprocessor
    printf("%s %.2f, %d\n", eq, *conta, file_no);
    //after fininsh process
    state=0;
    write(fd_state[1], &state, sizeof(int));
    if(state_end==-1){
      exit(0);
    }
    //while(1) fin
  }
  state_end=-1;
  write(fd_state_end[1], &state_end, sizeof(int));
  wait(NULL);
  return 0;
}
