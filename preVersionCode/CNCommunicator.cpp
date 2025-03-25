#include "CNCommunicator.hpp"

int CNCommunicator::CNcount=0;

CNCommunicator::CNCommunicator(void){
  CNCommunicator::CNcount++;
  if(CNcount!=1){
    errException("CNCommunciator should be Singleton").writeLog("CNCommunicator");
  }
  
  logException("CNCommunicator Constructed").writeLog("CNCommunicator");
}

CNCommunicator::~CNCommunicator(void){
  if(this->CNcount!=0){
    this->sendSignal(2);
  }
  logException("CNCommunicator Destructed").writeLog("CNCommunicator");
}

void CNCommunicator::sendSignal(int&& signum){
  if(!this->status){
    errException("Signal Fork should be called firstly").writeLog("CNCommunicator");
    exit(1);
  }
  
  if(signum==1){
    kill(this->pid, _SIGUSR1);
  } else if(signum==2){
    kill(this->pid, _SIGUSR2);
    this->CNcount--;
  } else {
    std::string strSignum=std::to_string(signum);
    errException("Signal 10, 12 only variable["+strSignum+"]").writeLog("CNCommunicator");
    exit(1);
  }
}

void CNCommunicator::CNfork(std::string input){
  this->path=input;
  
  this->pid=fork();
  if(this->pid==-1){
    exit(1);
  }
  
  if(this->pid==0){
    const char* command=".codes/cleaner";
    execl(command, command, this->path.c_str(), (char*)0);
    errException("Forked child").writeLog("CNCommunicator");
    exit(1);
  }
  
  logException("CNCommunicator Build Complete").writeLog("CNCommunicator");
  
  this->status=true;
}
