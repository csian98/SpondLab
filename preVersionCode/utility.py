import os
import numpy as np
import torch
import pandas as pd
import re
import time

ATOM=["Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi"]

atomN=80
maxDepth=40
validSpan=1000

#['0', '/Sc0.336842/P0.473684/Yb0.189474/', '35.330000']
def translate(infoList):
  target=np.zeros(atomN)
  partition=infoList[1].split('/')[1:-1]
  for part in partition:
    point=part.find('.')-1
    atom=part[0:point]
    ratio=float(part[point:])
    target[ATOM.index(atom)]=ratio
  
  target=torch.tensor(target)
  contaNorm=torch.tensor(float(infoList[2])/maxDepth).view(1)
  
  return contaNorm, target

def pthCheck():
  pthPath="pth/"
  dirList=os.listdir(pthPath)
  # 1000 to 1
  # XPSdnn000000.pth
  dirList=sorted(dirList, key=lambda value: int(value[6:-4]))
  if len(dirList)==0: return 0
  else: return int(dirList[-1][6:-4])

def validBuildTarget(label):
  equation=label[0].split('/')[1:-1]
  qTar=torch.zeros(atomN)
  for eq in equation:
    index=ATOM.index(re.sub(r'[0-9]+', '', eq))
    quant=float(re.sub(r'[^0-9]+', '', eq))
    qTar[index]=quant/100
  
  
  contaDepth=float(label[1][1:-1])/maxDepth
  cTar=torch.tensor(contaDepth).view(1)
  
  return cTar, qTar

def validation(model, loss1, loss2):
  validPath="validation/"
  dirList=os.listdir(validPath)
  
  model.eval()
  
  lossSum1=0; lossSum2=0
  for file in dirList:
    data=pd.read_csv(validPath+file, sep=' ', header="infer")
    label=data.columns[1:]
    validRaw=torch.tensor(data.values[:, 1], dtype=torch.float32).view(1, -1)
    
    cOut, qOut=model(validRaw)
    cTar, qTar=validBuildTarget(label)
    
    l1=loss1(cTar, cOut)
    l2=loss2(qTar, qOut)
    
    lossSum1+=l1.detach()
    lossSum2+=l2.detach()
  
  return lossSum1, lossSum2

def lossListAppend(sum1, sum2):
  lossesPath=".tmp/losses/"
  
  try:
    fp=open(lossesPath+"loss1.txt", "r")
    text=fp.read()
    if text=="":
      loss1=[]
      fp.close()
    else:
      loss1=list(map(float, text[1:-1].split(',')))
      fp.close()
  except:
    fp=open(lossesPath+"loss1.txt", "w"); fp.close()
    loss1=[]
  
  try:
    fp=open(lossesPath+"loss2.txt", "r")
    text=fp.read()
    if text=="":
      loss2=[]
      fp.close()
    else:
      loss2=list(map(float, text[1:-1].split(',')))
      fp.close()
  except:
    fp=open(lossesPath+"loss2.txt", "w"); fp.close()
    loss2=[]
  
  loss1.append(sum1); loss2.append(sum2)
  
  fp=open(lossesPath+"loss1.txt", "w")
  fp.write(str(loss1))
  fp.close()
  fp=open(lossesPath+"loss2.txt", "w")
  fp.write(str(loss2))
  fp.close()

def excLogger(inputStr):
  logPath=".tmp/log/"
  strTime=time.strftime("%c", time.localtime(time.time()))
  sentence='['+strTime+"] "+inputStr+'\n'
  
  fp=open(logPath+"pyLog.txt", "a")
  fp.write(sentence)
  fp.close()
