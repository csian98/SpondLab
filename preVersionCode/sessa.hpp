#pragma once

#include <string>
#include <string_view>
#include <cstdio>
#include <cassert>

#include <thread>
#include <mutex>
#include <fstream>

#include "utility.hpp"
#include "configReader.hpp"
#include "filesystem.hpp"
#include "exception.hpp"
#include "IPC2py.hpp"
#include "CNCommunicator.hpp"

/**
 * @brief SessaInfo used in class Sessa
 * final input values of sessa.exe
 */
struct SessaInfo{
  /**
   * @brief SessaInfo ctor
   *
   * @param std::string ; equation completed(upto 5 atoms, ratio sum to 100)
   * @param double  ; conta depth(0-40Å)
   */
  SessaInfo(std::string, double);
  
  std::string equation;
  
  double contaDepth;
};

/**
 * @brief Sessa is main class to handle generator
 */
class Sessa final{
public:
  /**
   * @brief Sessa ctor
   * initialize randomGenerator random
   * call sessaConnect()
   * @param
   * logged data number
   */
  Sessa(void);
  
  /**
   * @brief Sessa dctor
   * call sessaDisconnect()
   */
  ~Sessa();
  
  /**
   * @brief Sessa Operator Function
   * subRoutine methods launch with multithread
   *
   * @return
   * when reach targetNum return false, else return true
   */
  bool dataWrite(void);
  
  /**
   * @brief Just Call dataWrite Methods
   *
   * @return same as dataWrite Methods
   */
  bool operator()(void);
  
  static void writeStatus(int sigNum);
private:
  void sessaConnect(void);
  
  void sessaDisconnect(void);
  
  /**
   * @brief Reconnect Sessa.exe
   * Cause of memory Leakage periodically reconnect sessa.exe
   */
  void Reconnect(void);
  
  /**
   * @brief decide Exit condition
   *
   * @return inner counter reach targetNum retrun false to escape loop
   *    else return true to keep loop
   */
  bool insideCounter(void);
  
  /**
   * @brief from AtomRatio make SessaInfo which contains
   *    infomation for sessa.exe directly entered
   *
   * @param AtomRatio build AtomRatio with randomGenerator
   *
   * @return SessaInfo fully contain infomation go into sassa.exe
   */
  SessaInfo eqTransform(const AtomRatio&);
  
  /**
   * @brief Randomly shift peaks
   *    following randomly shifting rules describe on config file
   */
  void randomShift(void);
  
  /**
   * @brief thread called from dataWrite( or opeator() ) as Multi thread
   * before communcate with main.py normalize raw data(RawInfo class)
   *
   * @param SessaInfo require SessaInfo
   */
  void subRoutine(const SessaInfo&);
  
  void saveData(double*, std::string&);
  
  /**
   * @brief Sessa.exe is instable from multi tasking environment
   * inorder to minimize instable problem require mutex lock
   */
  std::mutex mtx;
  
  /**
   * @brief Communicate Raw data
   *    and randomly build infomation(contaDepth, equation)
   *
   * declared on IPC2py.hpp
   */
  Communicator communicator;

  /**
   * @brief Sessa.exe Pipeline
   */
  FILE* fp;
  
  /**
   * @brief When Init Process read/write config file
   * initial config setting on configReader.cpp
   */
  ConfigReader configreader;
  
  /**
   * @brief data structrue which contain path informations
   * decalred on filesystem.hpp
   */
  PathManager pathmanager;
  
  /**
   * @brief randomly build information(uniformly)
   * random atom number 1-5
   * contamination Depth 0-40A
   * random atom ratio 0-1 X (random atom number); sum to 1
   * declared on utility.hpp
   */
  randomGenerator random;
  
  long long int inCounter=0;
  
  static long long int prCounter;
  
  bool dataSave=false;
  
  int existData=0;
  
  CNCommunicator cnCommunicator;
};
