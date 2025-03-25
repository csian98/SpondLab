#!/opt/homebrew/Caskroom/miniforge/base/envs/tf_38/bin/python
import numpy as np
import torch

class DNN(torch.torch.nn.Module):
  def __init__(self, inputsz, outputsz=80):
    super(DNN, self).__init__()
    self.inputsz=inputsz
    self.outputsz=outputsz
    
    # 1
    self.cv1=torch.nn.Sequential(
      torch.nn.Conv1d(1, 1, 7, 1, "same"),
      torch.nn.ReLU(),
      torch.nn.Dropout(0.2)
    )
    self.cv2=torch.nn.Sequential(
      torch.nn.Conv1d(1, 1, 11, 1, "same"),
      torch.nn.ReLU(),
      torch.nn.Dropout(0.2)
    )
    self.cv3=torch.nn.Sequential(
    torch.nn.Conv1d(1, 1, 21, 1, "same"),
    torch.nn.ReLU(),
    torch.nn.Dropout(0.2)
    )
    self.cv4=torch.nn.Sequential(
      torch.nn.Conv1d(1, 1, 41, 1, "same"),
      torch.nn.ReLU(),
      torch.nn.Dropout(0.2)
    )
    self.cvMerge=torch.nn.Sequential(
      torch.nn.Conv1d(4, 4, 5, 1, "same"),
      torch.nn.ReLU(),
      torch.nn.Dropout(0.2),
      torch.nn.AvgPool1d(4, None, 0)
    )
    # 2
    self.conta=torch.nn.Sequential(
      torch.nn.Linear(inputsz, 100),
      torch.nn.Sigmoid(),
      torch.nn.Dropout(0.2),
      torch.nn.Linear(100, 1),
      torch.nn.Sigmoid()
    )
    # 3
    self.quant=torch.nn.Sequential(
      torch.nn.Linear(inputsz+1, inputsz),
      torch.nn.ReLU(),
      torch.nn.Dropout(0.2),
      torch.nn.Linear(inputsz, outputsz),
      torch.nn.Sigmoid()
    )
  
  def forward(self, x):
    y=torch.cat((self.cv1(x), self.cv2(x), self.cv3(x), self.cv4(x)), dim=1)
    y=self.cvMerge(y)
    y=torch.flatten(y)
    y_conta=self.conta(y)
    y=torch.cat((y, y_conta))
    y_quant=self.quant(y)
    sum=y_quant.sum()
    y_quant/=sum
    return (y_conta, y_quant)
