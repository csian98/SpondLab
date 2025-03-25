#!/opt/homebrew/Caskroom/miniforge/base/envs/tf_38/bin/python

import sys
import numpy as np
from scipy.ndimage.filters import gaussian_filter1d
import matplotlib.pyplot as plt

if __name__=='__main__':
  print(sys.argv[0])
  fd=open(sys.argv[1], 'rt')
  label=[]
  data=[]
  index=fd.readline()[:-1]
  for line in fd:
    line=line.split(' ')
    label.append(float(line[0]))
    data.append(float(line[1]))
  
  fd.close()
  filtered_data=gaussian_filter1d(data, 1)
  
  fd=open(sys.argv[1], 'wt')
  print(index, file=fd)
  for i in range(len(label)):
    print("%.4f %e" %(label[i], data[i]), file=fd)
  fd.close()
