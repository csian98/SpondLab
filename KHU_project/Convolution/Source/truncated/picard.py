# Picard plot
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

UB=abs(U.T.dot(before))
UBS=UB/D

plt.subplot(131)
plt.plot(D[:15])
plt.title("singular", "k-")

plt.subplot(132)
plt.plot(UB[:15], "rx")
plt.title("U.T*b")

plt.subplot(133)
plt.plot(UBS[:15], "b+")
plt.title("U.T*b/singular")
plt.show()
