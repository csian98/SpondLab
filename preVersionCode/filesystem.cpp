#include "filesystem.hpp"

CheckFile::CheckFile(std::string file): file(file), path(file) {}

bool CheckFile::check(void){
  return std::filesystem::exists(this->path);
}

// throw err
void CheckFile::waitFile60(void){
  std::chrono::system_clock::time_point start=std::chrono::system_clock::now();
  while(!this->check()){
    auto btw=std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now()-start);
    if(btw.count()>60) throw errException("Wait More than 60 sec");
  }
}

void CheckFile::rmFile(void){
  std::filesystem::remove(this->path);
}

std::string CheckFile::getFile(void) const{
  return this->file;
}

PathManager::PathManager(void){
  this->curPath=std::filesystem::current_path();
  
  this->sessaLoad=".SESSA/SESSA.MAC_v2.2/sessa\\ v2.2.app/Contents/MacOS/SESSA\\ v2.2 -c";
  
  this->dumpPath=this->curPath/".tmp/dump/";
  
  this->checkDumpFile=CheckFile(dumpPath/"sam_peak.txt");
  
  this->checkSpcFile=CheckFile(dumpPath/"reg1.spc");
}

std::string PathManager::getSessaLoad(void) const {
  return this->sessaLoad;
}

std::string PathManager::getCurPath(void) const {
  return static_cast<std::string>(this->curPath);
}

std::string PathManager::getDumpPath(void) const {
  return static_cast<std::string>(this->dumpPath);
}

void PathManager::dumpClear(void){
  std::filesystem::directory_iterator itr(this->dumpPath);
  
  for(; itr!=std::filesystem::end(itr); ++itr){
    const std::filesystem::directory_entry& entry=*itr;
    std::filesystem::remove(entry.path());
  }
}

bool PathManager::checkDumpFileExist(void){
  return this->checkDumpFile.check();
}

void PathManager::waitDumpFile60(void){
  this->checkDumpFile.waitFile60();
}

std::string PathManager::getDumpFilePath(void) const{
  return this->checkDumpFile.getFile();
}

void PathManager::rmDumpFile(void){
  this->checkDumpFile.rmFile();
}

bool PathManager::checkSpcFileExist(void){
  return this->checkSpcFile.check();
}

void PathManager::waitSpcFile60(void){
  this->checkSpcFile.waitFile60();
}

std::string PathManager::getSpcFilePath(void) const{
  return this->checkSpcFile.getFile();
}

void PathManager::rmSpcFile(void){
  this->checkSpcFile.rmFile();
}

void PathManager::makeDataDir(int dirNum){
  std::filesystem::path dataPath="./data/"+std::to_string(dirNum);
  if(!std::filesystem::create_directory(dataPath)){
    errException("create Directory Error").writeLog("FS");
  }
}

int PathManager::dataCount(void){
  std::filesystem::path dataPath("./data/");
  std::filesystem::directory_iterator itrD(dataPath);
  
  int maxDirNum=0, maxFileNum=0;
  
  while(itrD!=std::filesystem::end(itrD)){
    const std::filesystem::directory_entry& entry=*itrD;
    std::string tmp=entry.path().filename();
    if(tmp[0]=='.'){
      itrD++;
      continue;
    }
    int dirNum=std::stoi(entry.path().filename());
    if(maxDirNum<dirNum){
      maxDirNum=dirNum;
    }
    itrD++;
  }
  
  if(!maxDirNum) return maxFileNum;
  
  dataPath=dataPath/std::to_string(maxDirNum);
  std::filesystem::directory_iterator itrF(dataPath);
  
  while(itrF!=std::filesystem::end(itrF)){
    const std::filesystem::directory_entry& entry=*itrF;
    std::string tmp=entry.path().filename();
    if(tmp[0]=='.'){
      itrF++;
      continue;
    }
    int fileNum=std::stoi(entry.path().filename());
    if(maxFileNum<fileNum){
      maxFileNum=fileNum;
    }
    itrF++;
  }
  return maxFileNum;
}
