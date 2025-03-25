from dnnV2 import *

def HighPassFilter(y_true, y_pred):
  loss=y_true**2*(y_true-y_pred)**2
  return loss.sum()

if __name__=="__main__":
  inputsz=2048; outputsz=80
  model=DNN(inputsz)
  optim=torch.optim.Adam(model.parameters(), lr=1e-4)
  loss1=torch.nn.MSELoss()
  loss2=HighPassFilter


