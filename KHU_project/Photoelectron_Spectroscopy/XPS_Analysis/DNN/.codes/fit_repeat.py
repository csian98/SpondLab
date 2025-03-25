import os
import pandas as pd
import numpy as np
import re
from dnn import *

ATOM=["Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi"]

def label_0(form):
  global ATOM
  lab=np.zeros(ATOMS, float)
  form=form.split('/')
  form=form[1:-1]
  for ff in form:
    ind=ATOM.index(re.sub(r'[0-9]+', '', ff))
    lab[ind]=float(re.sub(r'[^0-9]+', '', ff))/100
  return lab

def label_1(depth):
  # 0-40 to 0-1
  return (depth-0)/40

###
data_path="./../../Generator/data_set/"

dir=os.listdir(data_path)

for i in range(100):
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
      print("[%d/100] %s" %(i, ff))
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

print("TRAINING WITH ALL DATA")
model.save_weights("XPS_weights_repeat100.h5")
