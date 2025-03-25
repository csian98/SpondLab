#include <string>
#include <sstream>
#include <vector>
#include <filesystem>

#include <unistd.h>
#include <signal.h>

#include "exception.hpp"

#define _SIGUSR1 10
#define _SIGUSR2 12

namespace sig{
  static bool status;
};

// KEEP MODE
void sigusr_rec1(int sig_naum){
  logException("CLN SIGUSR1 received").writeLog("CLN");
}
// EXIT MODE
void sigusr_rec2(int sig_num){
  logException("CLN SIGUSR2 received").writeLog("CLN");
  sig::status=false;
}

class Cleaner{
public:
  Cleaner(std::string fullPath){
    this->splitPath(fullPath);
    this->writeOrigState();
  }

  virtual ~Cleaner(void)=default;

  bool clean(void){
    std::string now;
    int state=0;
    for(std::string addPath: this->pathRoute){
      now=now+addPath+"/";
      std::filesystem::directory_iterator itr(now);
      while(itr!=std::filesystem::end(itr)){
        const std::filesystem::directory_entry& entry=*itr;
        if(std::find(this->origState.begin(), this->origState.end(), entry.path())==this->origState.end()){
          if(isFile(entry.path())){
            std::filesystem::remove(entry.path());
            state=1;
          }
        }
        itr++;
      }
    }
    return state;
  }
private:
  bool isFile(std::filesystem::path path){
    return std::filesystem::is_regular_file(path);
  }

  void splitPath(std::string fullPath){
    std::stringstream ss(fullPath);
    std::string buffer;
    while(std::getline(ss, buffer, '/')){
      this->pathRoute.push_back(buffer);
    }
  }

  void writeOrigState(void){
    std::string now;
    for(std::string addPath: this->pathRoute){
      now=now+addPath+"/";
      std::filesystem::directory_iterator itr(now);

      while(itr!=std::filesystem::end(itr)){
        const std::filesystem::directory_entry& entry=*itr;
        this->origState.push_back(entry.path());
        itr++;
      }
    }
    std::string userName=getlogin();
    this->origState.push_back("/Users/"+userName+"/SESSA v2-"+userName);
  }

  std::vector<std::string> pathRoute;

  std::vector<std::string> origState;
};

int main(int argc, char* argv[]){
  signal(_SIGUSR1, sigusr_rec1);
  signal(_SIGUSR2, sigusr_rec2);
  sig::status=true;
  
  std::string path=std::string(argv[1]);
  
  logException("Initiate Cleaner: "+path).writeLog("CLN");
  errException("Initiate Cleaner: "+path).writeLog("CLN");
  
  Cleaner cleaner(path);
  
  do{
    pause();
    
    if(cleaner.clean()){
      logException("CLN Operated").writeLog("CLN");
    }
  } while(sig::status);
  
  logException("Destruct Cleaner").writeLog("CLN");
  errException("Destruct Cleaner").writeLog("CLN");

  return 0;
}
