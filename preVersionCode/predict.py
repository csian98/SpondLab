#!/opt/homebrew/Caskroom/miniforge/base/envs/tf_38/bin/python

from dnnModel import *
from utility import *

import sys
import torch
import numpy as np
import matplotlib.pyplot as plt

def drawGraph(output):
  contaDepth=output[0].detach()*40
  plt.title("C5O1 contamination depth: %.2f" %contaDepth)
  plt.xlabel(ATOM)
  ##

if __name__=="__main__":
  existLabel=False
  labelSentence=""
  
  if len(sys.argv)!=3:
    excLogger("predict.py require 2 arguments")
  
  try:
    dnn=torch.load(sys.argv[1])
  except:
    excLogger("Not Compatible h5py")
  
  try:
    input=np.loadtxt(sys.argv[2])
    input=torch.tensor(input, dtype=torch.float32).view(1, -1)
  except:
    excLogger("Not Compatible data file")
  
  with open(sys.argv[2]) as fp:
    firstLine=fp.readline()
    if firstLine[0]=='#':
      existLabel=True
      labelSentence=firstLine[2:]
      
  output=dnn(input)
  
  # require modify
  with open(".tmp/dump2/predict.txt", "w") as fp:
    print(labelSentence, file=fp)
    print(output, file=fp)
