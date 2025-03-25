import sysv_ipc
import struct
import numpy as np
import torch

class shrMemory:
  def __init__(self, shrKey):
    while True:
      try:
        self.shrRaw=sysv_ipc.SharedMemory(3274)
        self.shrInfo=sysv_ipc.SharedMemory(3274+1)
        break
      except:
        pass
  
  def readRaw(self):
    data=self.shrRaw.read()
    raw=torch.zeros(2048)
    for i in range(2048):
      raw[i]=struct.unpack('d', data[8*i:8*(i+1)])[0]
    
    return raw.view(1, -1)
  
  # 1 for check 2 for exit
  def waitInfo(self):
    while True:
      init=self.shrInfo.read().decode()[0]
      if(init=='1' or init=="2"): break
    if(init=="2"): return False
    else: return True
  
  def readInfo(self):
    label=self.shrInfo.read()
    listOflabel=label.decode().split('\t')[:-1]
    return listOflabel
  
  def writeInfo(self):
    self.shrInfo.write("0")
