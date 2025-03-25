#pragma once

#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <utility>

#include "exception.hpp"

/**
 * @brief ConfigReader class
 * be used as a Sessa member
 */
class ConfigReader{
public:
  ConfigReader(void);
  
  virtual ~ConfigReader(void)=default;
  
  /**
   * @brief Initially used to get config setting values
   *
   * @param std::string config index
   * when required incorrect index immediately exit process
   * able to terminate process without resource freed(ex shared memory)
   *
   * @return config corresponding values
   */
  std::string getValue(std::string) const;
private:
  /**
   * @brief read Config file
   * when Config file index out of lists ignoreing it
   */
  void readConfig(void);

  /**
   * @brief write Config file
   * reWrite config file as setted index
   */
  void writeConfig(void);
  
  /**
   * @brief map for config index and value
   */
  std::unordered_map<std::string, std::string> config;
  
  std::filesystem::path configFile;
};
