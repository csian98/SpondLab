#!/opt/homebrew/Caskroom/miniforge/base/envs/tf_38/bin/python
from dnn import *
import pandas as pd
import re
import matplotlib.pyplot as plt
import sys

ATOM=["Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi"]

EPOCH=200
MAX_DEPTH=40    # [Å]
tolerence=0.03

path_ri="./../../Generator/relative/T.txt"
T=pd.read_csv(path_ri, header=None)[0]

def label_0(form):
  global ATOM, T
  lab=np.zeros(ATOMS, float)
  form=form.split('/')
  form=form[1:-1]
  sum_qT=0
  ind_l=[]
  y_l=[]
  for ff in form:
    ind_l.append(ATOM.index(re.sub(r'[0-9]+', '', ff)))
    y_l.append(float(re.sub(r'[^0-9]+', '', ff))/100)
    sum_qT+=y_l[-1]*T[ind_l[-1]]
  for i in range(len(form)):
    lab[ind_l[i]]=y_l[i]*T[ind_l[i]]/sum_qT
  return lab

def label_1(depth):
  # 0-40 to 0-1
  return (depth-0)/40

def pred_trans(pred, file=sys.stdout, string=False):
  global MAX_DEPTH
  _depth=pred[0]
  _depth*=MAX_DEPTH
  _tmp=pred[1][0]
  _rat=_tmp.copy()
  _dic={}
  _ss=_tmp.sum()
  for i in range(ATOMS):
    _rat[i]=_tmp[i]/T[i]/_ss
  
  _rat=_rat/_rat.sum()
  for i in range(ATOMS):
    if _rat[i]>tolerence:
      _dic[ATOM[i]]=_rat[i]*100
  _dic=dict(sorted(_dic.items(), key=lambda item: item[1], reverse=True))
  if(not string):
    print("XPS Analysis result", file=file)
    for keys in _dic.keys():
      print("%s : %.1f %%" %(keys, _dic[keys]), sep="", file=file)
    print("C5O contamination : %.2f [A]\n" %_depth, sep="", file=file)
  if(string):
    ss="/"
    for keys in _dic.keys():
      ss+="%s%d/"%(keys, _dic[keys])
    ss+=" [%.2f]"%_depth
    return ss

if __name__=="__main__":
  model.load_weights("XPS_weights_11.h5")
  
  sep=' '
  if(len(sys.argv)==1):
    print("\n\n INPUT FORMAT ERROR")
    print("./predict [predict data path]\n")
    exit()
  
  ## FILE NAME
  data_path=sys.argv[1]
  try:
    data=pd.read_csv(data_path, sep='\t', header=None)
  except:
    print("Not permitted format")
    exit()
  
  attr=data.values[:, [0, 1]]
  attr_max=max(attr[:, 1]); attr_min=min(attr[:, 1])
  if(attr_max!=1 or attr_min!=0):
    attr[:, 1]=(attr[:, 1]-attr_min)/(attr_max-attr_min)
  
  attr=attr[:, 1].reshape([1, INPUT_SZ, 1])
  
  pred=model.predict(attr)
  pred_label="Pred: %s"%(pred_trans(pred, string=True))
  
  plt.plot(pred[1][0], 'o', color="#881824", label=pred_label, alpha=0.5)
  plt.legend()
  plt.title("Predict: %.2f" %(pred[0][0]/MAX_DEPTH))
  plt.show()
