#pragma once

#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <memory.h>   // for Ubuntu Linux

#include <string>
#include <string_view>

#include "exception.hpp"

/**
 * @brief Key value for SharedMemory
 * when changed it, also need to change py's one on main.py
 */
static const int setKey=3274;

/**
 * @brief shrMemory class
 * included on Communicator Class(for shrRaw, shrInfo)
 */
class shrMemory{
public:
  shrMemory(const int key, const int memSize=128);
  
  virtual ~shrMemory(void);
  
  /**
   * @brief write addr(size) on shmemory
   * size should be smaller than memSize
   *
   * @param addr, size
   */
  void writeShr(void*, int size=128);
 
  /**
   * @brief read shrmemory on addr
   *
   * @param addr
   */
  void readShr(void*);
  
private:
  void initShr(void);
  
  void exitShr(void);

  int shmid;
  
  void* shmaddr;
  
  const int key, memSize;
};

/**
 * @brief Communicator
 * Communicator take role to fork main.py process(CTOR only once in process)
 *
 * Be included by Sessa as member
 */
class Communicator{
public:
  Communicator(void);
  
  virtual ~Communicator(void);
  
  /**
   * @brief Init main.py
   * fork process(multi process)
   */
  void initPy(void);
  
  /**
   * @brief Terminate main.py
   * main.py terminate with shrInfo initial byte as 2
   */
  void exitPy(void);
  
  /**
   * @brief write normalized raw data on shrRaw
   *
   * @param addr
   * @param size
   */
  void writeRaw(double* addr, int size=2048);
  
  /**
   * @brief 
   *
   * @param std::string write Equation [(1-5) X (0-1)]
   * @param double write Contamination Depth[0-40]
   *    on shrInfo
   *
   * shrInfo initial byte protocol
   * 0: completed
   * 1: not completed
   * 2: terminate
   */
  void writeInfo(std::string, double);
  
  /**
   * @brief Wait until main.py complete training(spin lock)
   */
  void readAndWait(void);
  
private:
  shrMemory shrRaw;
  
  shrMemory shrInfo;
};
