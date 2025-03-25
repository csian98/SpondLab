#include "exception.hpp"

SessaException::SessaException(std::string_view errMsg): errMsg(errMsg){}

const char* SessaException::what() const noexcept{
  return this->errMsg.c_str();
}

std::string SessaException::localTime(void) const{
  std::chrono::system_clock::time_point now=std::chrono::system_clock::now();
  std::time_t nowTime=std::chrono::system_clock::to_time_t(now);
  tm* nowTm=std::localtime(&nowTime);
  
  char buffer[64];
  std::strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M:%S", nowTm);
  return std::string(buffer);
}

errException::errException(std::string_view errMsg): SessaException(errMsg){}

void errException::writeLog(std::string_view errType) const {
  std::ofstream fout(this->logPath/"err.txt", std::ios::app);
  std::string now=this->localTime();
  fout<<'['<<now<<"] ("<<errType<<") "<<this->errMsg<<'\n';
  fout.close();
}

logException::logException(std::string_view errMsg): SessaException(errMsg){}

void logException::writeLog(std::string_view logType) const {
  std::ofstream fout(this->logPath/"log.txt", std::ios::app);
  std::string now=this->localTime();
  fout<<'['<<now<<"] ("<<logType<<") "<<this->errMsg<<'\n';
  fout.close();
}
