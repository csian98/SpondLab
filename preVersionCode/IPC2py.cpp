#include "IPC2py.hpp"

shrMemory::shrMemory(const int key, const int memSize): key(key), memSize(memSize){
  this->initShr();
}

shrMemory::~shrMemory(void){
  try{
    this->exitShr();
  } catch(const std::exception& e) {
    // if shrMemory::exitShr called already
  }
}

void shrMemory::initShr(void){
  if((shmid=shmget(static_cast<key_t>(this->key), this->memSize, IPC_CREAT| IPC_EXCL| 0666))==-1){
    errException("Shared Memory Get Error [1]").writeLog("SHM");
    exit(1);
  }
  shmid=shmget(static_cast<key_t>(this->key), this->memSize, IPC_CREAT| 0666);
  if(shmid==-1) {
    errException("Shared Memory Get Error [2]").writeLog("SHM");
    exit(1);
  }
  
  shmaddr=shmat(this->shmid, static_cast<void*>(0), 0);
  if(shmaddr==reinterpret_cast<void*>(-1)){
    errException("Shared Memory at Error").writeLog("SHM");
    this->exitShr();
    exit(1);
  }
}

void shrMemory::exitShr(void){
  try{
    if(shmdt(shmaddr)==-1) throw errException("Shared Memory Detach Error");
  
    if(shmctl(this->shmid, IPC_RMID, 0)==-1) throw errException("Shared Memory Exit Error");
  } catch (const SessaException& e){
    e.writeLog("SHM");
    exit(1);
  }
  shmid=0;
  shmaddr=nullptr;
}

void shrMemory::writeShr(void* addr, int size){
  if(size>this->memSize){
    errException("Shared Memory Write OverFlow").writeLog("SHM");
    this->exitShr();
    exit(1);
  }
  
  memset(shmaddr, 0, this->memSize);
  
  memcpy(shmaddr, addr, size);
}

void shrMemory::readShr(void* addr){
  memcpy(addr, shmaddr, this->memSize);
}

Communicator::Communicator(void): shrRaw(setKey, sizeof(double)*2048), shrInfo(setKey+1, sizeof(char)*128){
  try{
    this->initPy();
  } catch(const SessaException& e){
    e.writeLog("FORK");
    this->~Communicator();
  }
}

Communicator::~Communicator(void){
    this->exitPy();
}

void Communicator::writeRaw(double* addr, int size){
  this->shrRaw.writeShr((void*)addr, sizeof(double)*2048);
}

void Communicator::writeInfo(std::string equation, double conta){
  std::string buffer="1\t"+equation+'\t'+std::to_string(conta)+'\t';
  
  this->shrInfo.writeShr((void*)(buffer.c_str()), buffer.length()+1);
}

void Communicator::readAndWait(void){
  char* buffer=new char[128];
  do{
    this->shrInfo.readShr((void*)buffer);
    if(buffer[0]=='0') break;
  }while(true);
  
  delete[] buffer;
}

void Communicator::initPy(void){
  pid_t pidPy;
  if((pidPy=fork())==-1){
    throw errException("Fork Error");
  }
  
  if(pidPy==0){
    execl(".codes/main.py", (char*)0);
    throw errException("Fork Execl Error");
  }
  logException("Initiate main.py").writeLog("FORK");
}
  
void Communicator::exitPy(void){
  std::string buffer="2\t";
  this->shrInfo.writeShr((void*)(buffer.c_str()), buffer.length()+1);
  logException("Exit main.py").writeLog("FORK");
}
