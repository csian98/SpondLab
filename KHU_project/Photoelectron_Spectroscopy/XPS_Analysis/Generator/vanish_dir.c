#define _CRT_NO_SECURE_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>

#define PASSWORD "2017103274"
#define BUFFER_SIZE 128
#define DIR_NUM 3

void vanish_dir(char* dir_path){
  DIR* tmp_dir;
  struct dirent* dir_ent;
  struct stat etr_stat;
  char* buf=(char*)calloc(BUFFER_SIZE, sizeof(char));
  if((tmp_dir=opendir(dir_path))==NULL){
      perror("opendir");
      exit(1);
  }
  while((dir_ent=readdir(tmp_dir))!=NULL){
    if(!strcmp(dir_ent->d_name, ".") || !strcmp(dir_ent->d_name, "..")){
      continue;
    }
    sprintf(buf, "%s/%s", dir_path, dir_ent->d_name);
    stat(buf, &etr_stat);
    if(S_ISDIR(etr_stat.st_mode)!=0){
      vanish_dir(buf);
      rmdir(buf);
      continue;
    }
    remove(buf);
  }
  closedir(tmp_dir);
}

/**
 * @file vanish_dir.c
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
  char path[PATH_MAX];
  char dir_path[DIR_NUM][PATH_MAX];
  int i;
  
  // password termios
  struct termios oflags, nflags;
  char password[BUFFER_SIZE];
  tcgetattr(fileno(stdin), &oflags);
  nflags=oflags;
  nflags.c_lflag &= ~ECHO;
  nflags.c_lflag |= ECHONL;
  if(tcsetattr(fileno(stdin), TCSANOW, &nflags)!=0){
    perror("tcstattr");
    exit(1);
  }
  
  printf("CLEAR DIRECTORY[INITALIZE]\n");
  printf("ENTER \"QUIT\" TO CANCEL\n");
  printf("PASSWORD:");
  fgets(password, sizeof(password), stdin);
  password[strlen(password)-1]=0;
  if(strcmp(password, "QUIT")==0){
    printf("UNDO\n");
    return 0;
  }
  if(strcmp(password, PASSWORD)!=0){
    printf("WRONG PASSWORD\n");
    if(tcsetattr(fileno(stdin), TCSANOW, &oflags)!=0){
      perror("tcsetattr");
      exit(1);
    }
    return 0;
  }
  
  if(tcsetattr(fileno(stdin), TCSANOW, &oflags)!=0){
    perror("tcsetattr");
    exit(1);
  }
  //
  
  if(!getcwd(path, sizeof(path))){
    perror("getcwd");
    exit(1);
  }
  
  sprintf(dir_path[0], "%s/data_set", path);
  sprintf(dir_path[1], "%s/.tmp_data/raw_data", path);
  sprintf(dir_path[2], "%s/.tmp_data/tmp", path);
  
  for(i=0; i<DIR_NUM; i++){
    printf("clear %s\n", dir_path[i]);
    vanish_dir(dir_path[i]);
  }
  return 0;
}
