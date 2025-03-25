import numpy as np
import torch
import torchsummary
import h5py

def highPassLoss(output, target):
  y1=torch.pow(target, 2)
  t=torch.sub(target, output)
  y2=torch.pow(t, 2)
  z=torch.multiply(y1, y2)
  loss=torch.sum(z)
  return loss

class XPSDNN(torch.torch.nn.Module):
  def __init__(self, inputsz=2048, atomsz=80):
    super(XPSDNN, self).__init__()
    self.inputsz=inputsz
    self.atomsz=atomsz
    
    # Denoising
    self.befConv=torch.nn.Sequential(
      torch.nn.Linear(self.inputsz, self.inputsz),
      torch.nn.ReLU()
    )
    
    convSize=[7, 11, 21, 41, 81]
    self.Conv1=torch.nn.Sequential(
      torch.nn.Conv1d(1, 1, convSize[0], 1, "same"),
      torch.nn.ReLU()
    )
    
    self.Conv2=torch.nn.Sequential(
      torch.nn.Conv1d(1, 1, convSize[1], 1, "same"),
      torch.nn.ReLU()
    )
    
    self.Conv3=torch.nn.Sequential(
      torch.nn.Conv1d(1, 1, convSize[2], 1, "same"),
      torch.nn.ReLU()
    )
    
    self.Conv4=torch.nn.Sequential(
      torch.nn.Conv1d(1, 1, convSize[3], 1, "same"),
      torch.nn.ReLU()
    )
    
    self.Conv5=torch.nn.Sequential(
      torch.nn.Conv1d(1, 1, convSize[4], 1, "same"),
      torch.nn.ReLU()
    )
    
    self.aftConv=torch.nn.Sequential(
      torch.nn.Conv1d(len(convSize), 4, 5, 1, "same"),
      torch.nn.ReLU(),
      torch.nn.AvgPool1d(4)
    )
    
    # Contamination
    self.contamination=torch.nn.Sequential(
      torch.nn.Linear(self.inputsz, 100),
      torch.nn.ReLU(),
      torch.nn.Dropout(0.5),
      torch.nn.Linear(100, 1),
      torch.nn.Sigmoid()
    )
     
    # Quantification
    self.quantification=torch.nn.Sequential(
      torch.nn.Linear(self.inputsz+1, self.inputsz),
      torch.nn.ReLU(),
      torch.nn.Dropout(0.5),
      torch.nn.Linear(self.inputsz, self.atomsz),
      torch.nn.Sigmoid()
    )
    
  def forward(self, x):
    y=self.befConv(x)
    y1=self.Conv1(y)
    y2=self.Conv2(y)
    y3=self.Conv3(y)
    y4=self.Conv4(y)
    y5=self.Conv4(y)
    y=torch.cat([y1, y2, y3, y4, y5], 0)
    y=self.aftConv(y)
    y=y.flatten()
    
    c=self.contamination(y)
    
    y=torch.cat([c, y], 0)
    y=self.quantification(y)
    y=y/y.sum()
    
    return c, y
