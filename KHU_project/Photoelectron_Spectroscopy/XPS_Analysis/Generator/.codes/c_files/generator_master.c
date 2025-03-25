#define _CRT_NO_SECURE_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <time.h>
#include <limits.h>

#define _SIGUSR1 10
#define _SIGUSR2 12

static time_t rec, now;
static int state=0;

void sigusr_rec1(int sig_num){
  time(&rec);
}

void sigusr_rec2(int sig_num){
  state=-1;
}

/**
 * @file generator_master.c
 * @author CSIAN
 * @version 2.0.0
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
  
  pid_t ppid=getppid();
  time(&rec); time(&now);
  
  printf("GENERATOR MASTER IS READY\n");
  
  while(!state){}
  state=0;
  while(1){
    time(&now);
    if(now-rec>30){
      printf("DETECTED GENERATOR FAULT\n");
      kill(ppid, _SIGUSR2);
      sleep(10);
      continue;
    }
    if(state==-1){
      break;
    }
  }
  printf("GENERATOR MASTER TERMINATE\n");
  return 0;
}
