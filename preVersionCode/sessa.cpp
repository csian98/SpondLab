#include "sessa.hpp"

long long int Sessa::prCounter=0;

void Sessa::writeStatus(int sigNum){
  logException("Signal to write Status").writeLog("SessaStatus");
  errException("Signal to write Status").writeLog("SessaStatus");
  
  std::ofstream fout(".tmp/dump2/cpp", std::ios::out);
  fout<<Sessa::prCounter;
  fout.close();
}

namespace ATOMS{
  const char* atoms[]={"Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi"};
  
  extern const int atomsN=80;
  
  extern const int XPSnrow=2048;
}

SessaInfo::SessaInfo(std::string equ, double depth): equation(equ), contaDepth(depth){}

Sessa::Sessa(void): random(ATOMS::atomsN){
  this->pathmanager.dumpClear();
  
  this->prCounter=std::stoll(this->configreader.getValue("InitNum"));
  
  this->cnCommunicator.CNfork(this->pathmanager.getDumpPath());
  
  std::string strSave=configreader.getValue("DataSave");
  if( !strSave.compare("true") || !strSave.compare("t") ) this->dataSave=true;
  
  if(this->dataSave){
    logException("Data Save option ON").writeLog("CTOR");
    
    existData=this->pathmanager.dataCount();
  }
  
  logException("Initiate Seesa #"+std::to_string(this->prCounter)).writeLog("CTOR");
  errException("Initiate Sessa #"+std::to_string(this->prCounter)).writeLog("CTOR");
  
  this->sessaConnect();
}

Sessa::~Sessa(){
  this->sessaDisconnect();
  this->pathmanager.dumpClear();
  
  this->cnCommunicator.sendSignal(2);
  
  logException("Destruct Sessa #"+std::to_string(this->prCounter)).writeLog("DTOR");
  errException("Destruct Sessa #"+std::to_string(this->prCounter)).writeLog("DTOR");
}

void Sessa::sessaConnect(void){
  this->mtx.lock();
  
  if((this->fp=popen(this->pathmanager.getSessaLoad().c_str(), "w"))==NULL){
    perror("popen error");
  } else {
    fprintf(fp, "\\PROJECT RESET\n"); fflush(fp);
    fprintf(fp, "\\SOURCE SET %s\n", this->configreader.getValue("XPSsource").c_str()); fflush(fp);
    fprintf(fp, "\\SPECTROMETER SET RANGE \"400:1486\" REGION 0\n"); fflush(fp);
    fprintf(fp, "\\PREFERENCES SET DENSITY_SCALE MASS\n"); fflush(fp);
    fprintf(fp, "\\PREFERENCE SET OUTPUT SAMPLE \"false\"\n"); fflush(fp);
  }
  
  this->mtx.unlock();
}

void Sessa::sessaDisconnect(void){
  this->mtx.lock();
  
  fprintf(this->fp, "\\\n"); fflush(this->fp);
  fprintf(this->fp, "quit\n"); fflush(this->fp);
  pclose(fp);
  
  fp=nullptr;
  this->mtx.unlock();
}

void Sessa::Reconnect(void){
  this->sessaDisconnect();
  
  logException("Re-Integration of Sessa #"+std::to_string(this->prCounter)).writeLog("RECONN");
  
  this->sessaConnect();
}

bool Sessa::insideCounter(void){
  prCounter++; inCounter++;
  this->pathmanager.dumpClear();
  
  long long int targetNum=std::stoll(configreader.getValue("TargetNum"));
  int reconnectNum=std::stoi(configreader.getValue("SessaReconnect"));
  
  if(this->prCounter==targetNum){
    logException("Reach Target Data Count #"+std::to_string(targetNum)).writeLog("TARGET");
    return false;
  } else {
    if(this->inCounter%reconnectNum==0) this->Reconnect();
    return true;
  }
}

SessaInfo Sessa::eqTransform(const AtomRatio& atomratio){
  std::string equation="/";
  for(int i=0; i<atomratio.size; ++i){
    equation+= (ATOMS::atoms[atomratio.atom[i]]+std::to_string(atomratio.ratio[i])+'/');
  }
  return SessaInfo(equation, atomratio.contaDepth);
}

void Sessa::randomShift(void){
  PeakInfo peakinfo(pathmanager.getDumpFilePath());
  
  do{
    try{
      peakinfo.readData();
      break;
    } catch(const std::exception& e){
      peakinfo.reConnect();
    }
  } while(1);
  
  for(int i=0; i<peakinfo.getN(); ++i){
    double randomShift=0.0;
    if(peakinfo.isConta(i)){
      double sft=std::stod(this->configreader.getValue("ContaRdnSft"));
      randomShift=this->random.zeroMidRdn(sft);
    }
    else{
      double sft=std::stod(this->configreader.getValue("ChemRdnSft"));
      randomShift=random.zeroMidRdn(sft);
    }
    
    fprintf(fp, "\\SAMPLE PEAK SET CHEMSHIFT VALUE %.2lf PEAK %d\n", randomShift, i+1);
  }
}

void Sessa::subRoutine(const SessaInfo& sessainfo){
  RawInfo rawinfo(pathmanager.getSpcFilePath());
  
  do{
    try{
      rawinfo.readData();
      break;
    } catch(const std::exception& e){
      rawinfo.reConnect();
    }
  } while(1);
  
  if(this->dataSave){
    std::string writeDown=sessainfo.equation+"["+std::to_string(sessainfo.contaDepth)+"]";
    this->saveData(rawinfo.getRawAddr(), writeDown);
  }
  
  this->communicator.writeRaw(rawinfo.getRawAddr());
  
  this->communicator.writeInfo(sessainfo.equation, sessainfo.contaDepth);
  
  this->communicator.readAndWait();
}

bool Sessa::dataWrite(void){
  int _atomN=this->random.AtomN();
  
  AtomRatio _atomratio=this->random.Random(_atomN);
  
  SessaInfo _sessaInfo=this->eqTransform(_atomratio);
  
  this->mtx.lock();
  
  fprintf(fp, "\\SAMPLE RESET\n"); fflush(fp);
  fprintf(fp, "\\SAMPLE SET MATERIAL %s LAYER 0\n", _sessaInfo.equation.c_str()); fflush(fp);
  fprintf(fp, "\\SAMPLE ADD LAYER /C5/O/ THICKNESS %.2lf ABOVE 0\n", _sessaInfo.contaDepth); fflush(fp);
  
  this->mtx.unlock();
  
  this->mtx.lock();
  
  fprintf(this->fp, "\\\n"); fflush(this->fp);
  fprintf(fp, "\\PROJECT SAVE OUTPUT %s\n", this->pathmanager.getDumpPath().c_str()); fflush(fp);
  fprintf(this->fp, "\\\n"); fflush(this->fp);
  
  this->mtx.unlock();
  
  try{
    pathmanager.waitDumpFile60();
  } catch(const SessaException &e){
    e.writeLog("OVERTIME");
    
    this->cnCommunicator.sendSignal(1);
    
    this->Reconnect();
    return true;
  }
  
  this->randomShift();
  
  pathmanager.rmDumpFile();
  
  this->mtx.lock();
  
  // .tmp/dump/reg1.spc
  fprintf(fp, "\\MODEL SIMULATE\n"); fflush(fp);
  fprintf(fp, "\\MODEL SAVE SPECTRA \"%s\"\n", pathmanager.getDumpPath().c_str()); fflush(fp);
  
  this->mtx.unlock();
  
  std::thread subThread(&Sessa::subRoutine, this, _sessaInfo);
  subThread.detach();
  
  return this->insideCounter();
}

bool Sessa::operator()(void){
  return this->dataWrite();
}

void Sessa::saveData(double* raw, std::string& equation){
  this->existData++;
  int dirNum=existData/1000+1;
  
  if(this->existData%1000==1) pathmanager.makeDataDir(dirNum);
  
  std::string saveName="./data/"+std::to_string(dirNum)+"/"+std::to_string(existData);
  std::ofstream fout(saveName, std::ios::out);
  
  fout<<"# "<<equation<<'\n';
  for(int i=0; i<2048; ++i) fout<<raw[i]<<'\n';
  
  fout.close();
}
