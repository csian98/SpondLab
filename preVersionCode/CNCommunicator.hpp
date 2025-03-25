#pragma once

#include <string>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "exception.hpp"

#define _SIGUSR1 10
#define _SIGUSR2 12

class CNCommunicator{
public:
  CNCommunicator(void);
  
  virtual ~CNCommunicator(void);
  
  void CNfork(std::string);
  
  void sendSignal(int&&);
private:
  pid_t pid;
  
  static int CNcount;
  
  std::string path;
  
  bool status=false;
};
