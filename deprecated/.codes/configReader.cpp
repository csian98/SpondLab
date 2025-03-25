#include "configReader.hpp"

const std::vector<std::pair<std::string, std::string>> defaultConfig{
  {"InitNum", "0"},
  {"TargetNum", "20`000`000"},
  {"SessaReconnect", "1000"},
  {"XPSsource", "ALKA"},
  {"ChemRdnSft", "5"},
  {"ContaRdnSft", "0.5"},
  {"DataSave", "false"}
};

ConfigReader::ConfigReader(void){
  this->configFile=std::filesystem::current_path()/"config";
  for(auto iter=defaultConfig.begin(); iter!=defaultConfig.end(); ++iter){
    this->config[iter->first]=iter->second;
  }
  
  this->readConfig();
}

[[noreturn]] void Terminate(void){
  std::exit(1);
}

std::string ConfigReader::getValue(std::string key) const{
  try{
    auto iter=this->config.find(key);
    if(iter==this->config.end()){
      throw errException("No Config Element to Init");
    } else {
      return iter->second;
    }
  } catch(const SessaException& e){
    e.writeLog("CONFIG INIT");
  }
  Terminate();
}

void ConfigReader::readConfig(void){
  std::ifstream fin(this->configFile);
  std::string buffer;
  
  while(std::getline(fin, buffer)){
    std::string first, second;
    
    auto iter=buffer.find(':');
    first=buffer.substr(0, iter);
    second=buffer.substr(iter+1, buffer.length()-iter-1);
    
    first.erase(std::remove(first.begin(), first.end(), ' '));
    second.erase(std::remove(second.begin(), second.end(), ' '));
    
    try{
      if(this->config.find(first)==config.end()){
        std::string errMsg="No Config Element "+first;
        throw errException(errMsg);
      } else {
        this->config[first]=second;
      }
    } catch(const SessaException& e){
      e.writeLog("CONFIG");
    }
  }
  fin.close();
  this->writeConfig();
}

void ConfigReader::writeConfig(void){
  std::ofstream fout(this->configFile);
  
  for(auto iter=defaultConfig.begin(); iter!=defaultConfig.end(); ++iter){
    fout<<iter->first<<" : "<<this->config[iter->first]<<'\n';
  }
  fout.close();
}
