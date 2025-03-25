#define _CRT_NO_SECURE_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/stat.h>
#include <time.h>
#include <limits.h>

#define _SIGUSR1 10

void sigusr_rec(int sig_num){
  printf("RECEIVE SIGUSR1 FOR sigwait.c");
}

int main(void){
  sigset_t set;
  sigemptyset(&set);
  sigprocmask(SIG_BLOCK, &set, NULL);
  pid_t pid;
  int sig;
  int *sigptr=&sig;
  
  if(sigaddset(&set, _SIGUSR1)==-1){
    perror("sigaddset");
    exit(1);
  }
  
  while(1){
    if(sigwait(&set, sigptr)==-1){
      perror("sigwait");
      exit(1);
    }
    if((pid=fork())==-1){
      perror("fork");
      exit(1);
    }
    if(pid==0){
      execl("generator", (char*)0);
      perror("execl_generator");
      exit(1);
    }
  }
}
