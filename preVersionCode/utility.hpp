#pragma once

#include <utility>
#include <fstream>
#include <filesystem>
#include <vector>
#include <random>

#include <limits>
#include <string>
#include <string_view>
#include <cmath>

#include "IPC2py.hpp"

/**
 * @brief AtomRatio contains randomly selected atoms(1-5) and their ratio
 *    and Contamination(C5O1) Depth 0-40
 *
 * built by randomGenerator::Random(int) methods
 */
struct AtomRatio{
  AtomRatio(int);
  virtual ~AtomRatio()=default;
  
  const int size;
  std::vector<int> atom;
  std::vector<double> ratio;
  double contaDepth;
};

/**
 * @brief randomGenerator included in Sessa as a member
 * initialize random_device when build instance,
 *    therefor only called once at beginning of process
 */
class randomGenerator{
public:
  randomGenerator(int);
  
  virtual ~randomGenerator()=default;
  
  int AtomN(void);
  
  AtomRatio Random(int);
  
  /**
   * @brief build rdn number around 0
   *
   * @param double random variable width
   *
   * @return random number between [-input, input)
   *
   * used to achieve shift values at Sessa::randomShift
   */
  double zeroMidRdn(double);

private:
  int atomsN;
  
  std::random_device rd;
};

/**
 * @brief class for PeakInfo and RawInfo
 * pure virtual class
 */
class Info{
public:
  /**
   * @brief read datas as each format
   */
  virtual void readData(void)=0;
  
  virtual int getN(void) const=0;
  
  /**
   * @brief reConnect file
   * To avoid to access incompleted files made by sessa.exe
   */
  virtual void reConnect(void)=0;

protected:
  virtual void skipLine(int)=0;
};

class PeakInfo: public Info{
public:
  PeakInfo(std::string);
  
  virtual ~PeakInfo(void);
  
  void reConnect(void) override;
  
  void readData(void) override;
  
  /**
   * @brief return sequence of peak is from C or O
   *
   * @param index
   *
   * @return true if index peak came from C or O, false if not
   */
  bool isConta(int index) const;
  
  int getN(void) const override;
  
private:
  void skipLine(int) override;
  
  std::ifstream fin;
  
  std::vector<bool> contaLabel;
  
  int peakN=0;
  
  /**
   * @brief label for sam_peak.txt format
   */
  const int lNum=2, beg=5;
  
  std::string PeakPath;
};

class RawInfo: public Info{
public:
  RawInfo(std::string);
  
  virtual ~RawInfo(void);
  
  void reConnect(void) override;
  
  void readData(void) override;
  
  int getN(void) const override;
  
  /**
   * @brief get raw[2048] dynamic memory address
   * used to transmit raw information to python codes through shared memory
   *
   * @return raw data initial address
   */
  double* getRawAddr(void) const;
  
private:
  void skipLine(int) override;
  
  /**
   * @brief Nomalize(zero to one) raw data
   */
  void z2oNorm(void);

  double* raw;
  
  std::ifstream fin;
  
  std::string rawPath;
  
  const int sz=2048;
  
  /**
   * @brief label for reg1.spc format
   */
  const int bLine=2;
};
