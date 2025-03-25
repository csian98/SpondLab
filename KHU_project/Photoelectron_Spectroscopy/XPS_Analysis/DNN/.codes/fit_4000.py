import os
import pandas as pd
import numpy as np
import re
from dnn import *
import matplotlib.pyplot as plt

# epoch use 200
EPOCH=30

ATOM=["Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi"]

path_ri="./../../Generator/relative/T.txt"
T=pd.read_csv(path_ri, header=None)[0]

#
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

#
def label_1(depth):
  # 0-40 to 0-1
  return (depth-0)/40


###
if __name__=="__main__":
  data_path="./../../Generator/data_4000/"
  dir=os.listdir(data_path)
  dir.sort()
  
  loss1_l=[]
  loss2_l=[]
  
  for epo in range(EPOCH):
    epoch=epo+1
    loss1=0; loss2=0
    for dd in dir:
      if dd==".DS_Store":
        continue
      print("\n", dd)
      dir_path=data_path+dd
      file=os.listdir(dir_path)[1:]
      file.sort()
      for ff in file:
        if ff==".DS_Store":
          continue
        file_path=dir_path+"/"+ff
        print("[%d/%d] %s" %(epoch, EPOCH, ff))
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
        history=model.fit(attr, [label2, label1], epochs=1)
        loss1+=history.history["dense_1_loss"][0]
        loss2+=history.history["dense_3_loss"][0]
    loss1_l.append(loss1)
    loss2_l.append(loss2)
  
  print("TRAINING WITH ALL DATA")
  model.save_weights("XPS_weights.h5")
