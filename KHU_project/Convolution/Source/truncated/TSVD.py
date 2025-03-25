import os
import numpy as np
import matplotlib.pyplot as plt

dir_path="/Users/csian/Desktop/CP/Conovlution/Source/truncated"
os.chdir(dir_path)

filter=np.loadtxt("filter.txt", dtype="float")
before=np.loadtxt("data.txt", dtype="float")
N=np.shape(before)[0]
Nf=np.shape(filter)[0]

toep=np.zeros([N, N])
for i in range(N):
  for j in range(Nf):
    if (i-Nf//2+j<0 or i-Nf//2+j>=N):
      continue
    toep[i, i-Nf//2+j]=filter[j]

U, D, V_t=np.linalg.svd(toep)

dir_path="/Users/csian/Desktop/CP/Conovlution/Source/truncated/k"
os.chdir(dir_path)

data_name="k_"

UB=U.T.dot(before)
UBS=UB/D

for k in range(1, 17):
  after=np.zeros([U.shape[0], ])
  for i in range(k):
    after+=UBS[i]*(V_t[i, :])
  
  tmp=data_name+str(k)+".txt"
  np.savetxt(tmp, after)
