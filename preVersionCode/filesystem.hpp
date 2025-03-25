#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <chrono>

#include "exception.hpp"

/**
 * @brief checking sessa.exe ouput files
 * used to check two files(same_peak.txt and reg1.spc in .tmp/dump)
 *
 * included in PathManager as a member(2 instances)
 */
class CheckFile{
public:
  CheckFile()=default;

  CheckFile(std::string);
  
  virtual ~CheckFile()=default;
  
  CheckFile(const CheckFile&)=default;
  
  /**
   * @brief check File existency
   *
   * @return true for file exist, false fo file non-exist
   */
  bool check(void);
  
  /**
   * @brief wait file for 60 seconds
   * cannot build file for 60 seconds deem sessa.exe error
   * immediately throw error to call Sessa::Reconnect methods to revive process
   */
  void waitFile60(void);
  
  /**
   * @brief remove file
   */
  void rmFile(void);
  
  /**
   * @brief get stroed file string
   *
   * @return Checkfile::file
   */
  std::string getFile(void) const;
  
private:
  std::string file;
  std::filesystem::path path;
};

/**
 * @brief contain various file paths
 * included various CheckFIle wrapper methods
 *
 * - Sessa.exe
 *   getSessaLoad
 *
 * - sam_peak.txt
 *   getDumpFilePath
 *   checkDumpFileExist
 *   waitDumpFile60
 *   rmDumpFile
 *
 * - reg1.spc
 *   checkSpcFileExist
 *   waitSpcFile60
 *   getSpcFilePath
 *
 * - etc
 *   getCurPath
 *   getDumpPath
 *   dumpClear
 *
 * included in Sessa as a member
 */
class PathManager{
public:
  PathManager(void);
  virtual ~PathManager()=default;
  
  std::string getSessaLoad(void) const;
  
  /**
   * @brief get Current Path(pwd)
   * In ordr to convey it to sessa.exe
   */
  std::string getCurPath(void) const;

  /**
   * @brief get Dump Path
   *
   * @return .tmp/dump
   */
  std::string getDumpPath(void) const;
  
  /**
   * @brief clear dump directory(.tmp/dump)
   * called Sessa::CTOR and Sessa::DTOR ; begin and end of process
   * called Sessa::insideCounter; every step
   */
  void dumpClear(void);
  
  bool checkDumpFileExist(void);
  
  void waitDumpFile60(void);
  
  std::string getDumpFilePath(void) const;
  
  void rmDumpFile(void);
  
  bool checkSpcFileExist(void);
  
  void waitSpcFile60(void);
  
  std::string getSpcFilePath(void) const;
  
  void rmSpcFile(void);
  
  int dataCount(void);
  
  void makeDataDir(int);
  
private:
  std::string sessaLoad;
  
  std::filesystem::path curPath;
  
  std::filesystem::path dumpPath;
  
  /**
   * @brief sam_peak.txt file checker
   */
  CheckFile checkDumpFile;
  
  /**
   * @brief reg1.spc file checker
   */
  CheckFile checkSpcFile;
};
