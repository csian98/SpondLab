import os
import pandas as pd
import numpy as np
import re
from dnn import *
import matplotlib.pyplot as plt
import sys

EPOCH=200
MAX_DEPTH=40    # [Å]
tolerence=0.03
INIT=0

ATOM=["Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi"]

path_ri=".\\T.txt""
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

def pred_trans(pred, file=sys.stdout):
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
  print("XPS Analysis result", file=file)
  for keys in _dic.keys():
    print("%s : %.1f %%" %(keys, _dic[keys]), sep="", file=file)
  print("C5O contamination : %.2f [A]\n" %_depth, sep="", file=file)

###
data_path=".\\data_merge\\"
dir=os.listdir(data_path)
dir=sorted(dir, key=lambda value: int(value[4:]))

loss1=[]
loss2=[]
valid_path=".\\data_valid\\"

model_path=".\\model\\"
model_li=os.listdir(model_path)
model_li=sorted(model_li, key=lambda value: int(value[12:-3]))

if __name__=="__main__":
  if len(model_li)!=0:
    model.load.weights(model_path+model_li[-1])
    print("Launch %s" %model_li[-1])
    fp=open(".\\loss1.txt", "r")
    loss1=map(float, fp.read()[1:-1].split(','))
    fp.close()
    fp=open(".\\loss2.txt", "r")
    loss2=map(float, fp.read()[1:-1].split(','))
    fp.close()
    INIT=len(model_li)
    assert INIT==len(loss1) and INIT==len(loss2)
    time.sleep(5)
  for i in range(INIT, EPOCH):
    for dd in dir:
      print("\n[%d/%d]%s:\t" %(i+1, EPOCH, dd), end="", flush=True)
      dir_path=data_path+dd
      file=os.listdir(dir_path)
      j=0
      for ff in file:
        if ff[0]=='.':
          continue
        if j%100==0:
          print("* ", end="", flush=True)
        file_path=dir_path+"/"+ff
        data=pd.read_csv(file_path, sep=' ', header="infer")
        lab=data.columns
        lab=lab[1:]
        # lab[0]= FORMULA   // label1
        # lab[1]= CONTAMINATION DEPTH   // label2
        label1=label_0(lab[0])
        label2=float(lab[1][1:-1])
        label2=label_1(label2)
        label2=np.array(label2)
        label2=label2.reshape([1, 1])
        label1=label1.reshape([1, ATOMS])
        attr=data.values[:, [0, 1]]
        attr=attr[:, 1].reshape([1, INPUT_SZ, 1])
        history=model.fit(attr, [label2, label1], verbose=0)
        j+=1
    l1=0
    l2=0
    file=os.listdir(valid_path)
    j=0
    print("\n[%d/%d]VALIDATION:\t" %(i+1, EPOCH), end="", flush=True)
    for ff in file:
      if ff[0]==".":
        continue
      if j%100==0:
          print("* ", end="", flush=True)
      file_path=valid_path+"\\"+ff
      data=pd.read_csv(file_path, sep=' ', header="infer")
      lab=data.columns
      lab=lab[1:]
      # lab[0]= FORMULA   // label1
      # lab[1]= CONTAMINATION DEPTH   // label2
      label1=label_0(lab[0])
      label2=float(lab[1][1:-1])
      label2=label_1(label2)
      label2=np.array(label2)
      label2=label2.reshape([1, 1])
      label1=label1.reshape([1, ATOMS])
      attr=data.values[:, [0, 1]]
      attr=attr[:, 1].reshape([1, INPUT_SZ, 1])
      history=model.evaluate(attr, [label2, label1])
      l1+=history[1]
      l2+=history[2]
      j+=1
    loss1.append(l1/j)
    loss2.append(l2/j)
    fp=open(".\\loss1.txt", "w")
    fp.write(loss1)
    fp.close()
    fp=open(".\\loss2.txt", "w")
    fp.write(loss2)
    fp.close()
    model.save_weights(".\\model\\XPS_weights_%d.h5" %(i+1))
  print("\nTRAINING WITH ALL DATA")
  model.save_weights(".\\model\\XPS_weights.h5")
  os.system("SHUTDOWN -s -t 60")
