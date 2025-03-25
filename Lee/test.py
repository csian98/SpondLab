import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os,sys,re
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
from tensorflow.keras import regularizers
from tensorflow.keras import losses
#from keras.utils.vis_utils import plot_model
from tensorflow.keras.layers import Attention, MultiHeadAttention
#from keras_multi_head import MultiHead
import seaborn as sns
from matplotlib.ticker import (MultipleLocator, AutoMinorLocator)

class Info:
    INPUT_SZ = 2048
    SPECIES = [
    "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P",
    "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni",
    "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr",
    "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te",
    "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Sm", "Eu", "Gd", "Tb",
    "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir",
    "Pt", "Au", "Hg", "Tl", "Pb", "Bi"
    ]
    NSPECIES=None
    T=None

Info.NSPECIES = len(Info.SPECIES)
apath = "./T.txt"
Info.T = pd.read_csv(apath, header=None)[0]

#
test=pd.read_csv("./data/output", sep="\t", header=None)
model=keras.models.load_model("./XPS_model1.h5")

input=np.array([test[1]])

ntry=200
aa=np.zeros((ntry), dtype=float)
bb=np.zeros((ntry, Info.NSPECIES), dtype=float)
for itry in range(ntry):
    pred, pred1=model.predict(input, verbose=0)
    aa[itry]=aa[itry]+pred[0]
    bb[itry, 0:Info.NSPECIES]=pred1[0:Info.NSPECIES]

plt.rcParams['figure.figsize'] = [12, 6]
#plt.rcParams.update({'errorbar.capsize': 2})
ecolors = ['magenta' for _ in range(Info.NSPECIES)]
t1sum=sum(bb.mean(axis=0))

ind = np.arange(Info.NSPECIES)
zlist=[]
for i in range(len(Info.SPECIES)):
    if np.mod(i,3) == 0 :
        zlist.append(Info.SPECIES[i])
    else:
        zlist.append(" ")

        
predvec=np.zeros(Info.NSPECIES)
prederr=np.zeros(Info.NSPECIES)

predvec[0:Info.NSPECIES] = bb.mean(axis=0)[0:Info.NSPECIES]
prederr[0:Info.NSPECIES] = bb.std(axis=0)[0:Info.NSPECIES]

for i in range(Info.NSPECIES):
    predvec[i]=predvec[i]*100.
    prederr[i]=prederr[i]*100.  #*1.96
fig, ax = plt.subplots()
plt.bar(ind, predvec, yerr=prederr,
        color='cyan', alpha=0.5, ecolor='magenta', capsize=2)

#plt.title('YS_PARK XPS output data', fontsize=20)
plt.xlabel('', fontsize=1)
plt.ylabel('Percentage (%)', fontsize=24)
#plt.xticks(ind, Info.SPECIES, fontsize=7)
plt.xticks(ind, zlist, fontsize=18)
ax.set_facecolor('ivory')
ax.yaxis.set_major_locator(MultipleLocator(10.))
ax.yaxis.set_minor_locator(AutoMinorLocator())
ax.tick_params(which='both', width=1)
ax.tick_params(which='major', length=4)
ax.tick_params(which='minor', length=2, color='gray')
plt.xticks(fontsize=16)
plt.yticks(fontsize=16)
if True:
    plt.savefig("./output.png")
plt.show()
