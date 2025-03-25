#pragma once

#include <exception>
#include <fstream>
#include <filesystem>
#include <iostream>

#include <string>
#include <string_view>
#include <chrono>
#include <ctime>

/**
 * @brief parent Exception
 * in order to use polymorphism when catching error
 *
 * pure virtual class
 */
class SessaException: public std::exception{
public:
  SessaException(std::string_view);
  
  virtual ~SessaException(void)=default;
  
  const char* what(void) const noexcept override;
  
  /**
   * @brief write log on non-volatile storage
   * .tmp/log/
   */
  virtual void writeLog(std::string_view) const=0;
  
protected:
  void setMsg(std::string_view);
  
  /**
   * @brief achieve localTime for logging messages
   *
   * @return 
   */
  std::string localTime(void) const;
  
  std::string errMsg;
  
  const std::filesystem::path logPath=".tmp/log";
};

/**
 * @brief error Exception
 */
class errException: public SessaException{
public:
  /**
   * @brief errException CTOR
   *
   * @param std::string_view error message
   */
  errException(std::string_view);
  
  virtual ~errException(void)=default;
  
  /**
   * @brief write error log (.tmp/log/err.txt)
   *
   * @param std::string_view error tag
   */
  virtual void writeLog(std::string_view) const override;
};

class logException: public SessaException{
public:
  /**
   * @brief logException CTOR
   *
   * @param std::string_view log message
   */
  logException(std::string_view);
  
  virtual ~logException(void)=default;
  
  /**
   * @brief write logging log (.tmp/log/log.txt)
   *
   * @param std::string_view logging tag
   */
  virtual void writeLog(std::string_view) const override;
};
