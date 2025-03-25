#include "utility.hpp"

AtomRatio::AtomRatio(int size): size(size), atom(size, 0), ratio(size, 0) {}

randomGenerator::randomGenerator(int atomsN): atomsN(atomsN){}

int randomGenerator::AtomN(void){
  std::mt19937 gen(this->rd());
  std::uniform_int_distribution<int> dis(1, 5);
  return dis(gen);
}

AtomRatio randomGenerator::Random(int N){
  std::mt19937 gen(this->rd());
  
  AtomRatio atomratio(N);
  
  std::uniform_int_distribution<int> atom(0, this->atomsN-1);
  std::uniform_int_distribution<int> rat(0, 100);
  std::uniform_int_distribution<int> depth(0, 4000);  // maxDepth fix?
  
  for(int i=0; i<N; ++i){
    int at=atom(gen);
    if(find(atomratio.atom.begin(), atomratio.atom.end(), at)!=atomratio.atom.end()){
      i--; continue;
    }
    atomratio.atom[i]=at;
    atomratio.ratio[i]=rat(gen);
  }
  
  double sum=0;
  for(double r: atomratio.ratio) sum+=r;
  for(int i=0; i<N; ++i) atomratio.ratio[i]/=sum;
  
  atomratio.contaDepth=static_cast<double>(depth(gen))/100;
  
  return atomratio;
}

double randomGenerator::zeroMidRdn(double maxSft){
  std::mt19937 gen(this->rd());
  
  int twoMax=static_cast<int>(maxSft*2*1000);
  std::uniform_int_distribution<int> rdn(0, twoMax);
  return static_cast<double>(rdn(gen))/1000-maxSft;
}

PeakInfo::PeakInfo(std::string PeakPath): PeakPath(PeakPath){
  fin.open(PeakPath);
  fin>>std::noskipws;
}

PeakInfo::~PeakInfo(void){
  this->fin.close();
}

void PeakInfo::skipLine(int line){
  for(int i=0; i<line; ++i) this->fin.ignore(128, '\n');
}

void PeakInfo::reConnect(void){
  this->fin.close();
  this->fin.open(this->PeakPath);
}

void PeakInfo::readData(void){
  skipLine(this->lNum-1);
  std::string buffer;
  std::getline(this->fin, buffer);
  int start, end;
  start=buffer.find(':'); end=buffer.length()-1;
  buffer=buffer.substr(start+1, end-start);
  
  this->peakN=std::stoi(buffer);
  
  this->contaLabel.assign(peakN, false);
  skipLine(this->beg-this->lNum-1);
  
  start=3;
  for(int i=0; i<this->peakN; ++i){
    if(i==101) ++start;
    std::getline(this->fin, buffer);
    buffer=buffer.substr(start, 2);
    
    if(buffer==" C" || buffer==" O") contaLabel[i]=true;
  }
}

bool PeakInfo::isConta(int index) const{
  return this->contaLabel[index];
}

int PeakInfo::getN(void) const{
  return this->peakN;
}

RawInfo::RawInfo(std::string rawPath): rawPath(rawPath){
  this->fin.open(rawPath);
  this->fin>>std::noskipws;
  
  this->raw=new double[this->sz];
}

RawInfo::~RawInfo(void){
  delete[] this->raw;
  
  this->fin.close();
}

void RawInfo::reConnect(void){
  this->fin.close();
  this->fin.open(this->rawPath);
}

void RawInfo::readData(void){
  this->skipLine(this->bLine-1);
  for(int i=0; i<this->sz; ++i){
    std::string buffer;
    std::getline(this->fin, buffer);
    int point=buffer.find(' ');
    buffer=buffer.substr(point+1);
    this->raw[i]=stod(buffer);
  }
  this->z2oNorm();
}

int RawInfo::getN(void) const {
  return this->sz;
}

double* RawInfo::getRawAddr(void) const {
  return this->raw;
}

void RawInfo::skipLine(int line){
  for(int i=0; i<line; ++i) this->fin.ignore(128, '\n');
}

void RawInfo::z2oNorm(void){
  double dmin=std::numeric_limits<double>::max();
  double dmax=std::numeric_limits<double>::min();
  
  for(int i=0; i<this->sz; ++i){
    if(this->raw[i]>dmax) dmax=this->raw[i];
    if(this->raw[i]<dmin) dmin=this->raw[i];
  }
  
  double gap=dmax-dmin;
  
  for(int i=0; i<this->sz; ++i){
    this->raw[i]=(this->raw[i]-dmin)/gap;
  }
}
