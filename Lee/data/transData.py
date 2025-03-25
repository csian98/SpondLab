import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def normalize(df):
  max0=max(df.iloc[:, 0]); min0=min(df.iloc[:, 0])
  df.iloc[:, 0]=(df.iloc[:, 0]-min0)/(max0-min0)
  
  return df.iloc[:, 0]

data=pd.read_csv("input", sep="\t", skiprows=3).iloc[:, [0, 2]]
data=data.set_index(data.columns[0])

data.iloc[:, 0]=normalize(data)

x=list(); y=list()

newData=data
index=np.logical_and(newData.index>400, newData.index<1486)
xpsIndex=pd.read_csv("sample", sep=" ", header=None).iloc[:, 0]

for i in range(len(xpsIndex)):
    x.append(xpsIndex[i])
    down=sum(data.index<xpsIndex[i])-1
    downV=data.iloc[down, 0]; upV=data.iloc[down+1, 0]
    xinter=data.index[down+1]-data.index[down]
    slope=(upV-downV)/xinter
    y.append(downV+slope*(xpsIndex[i]-data.index[down]))

ymax=max(y); ymin=min(y)
y=(y-ymin)/(ymax-ymin)

df=pd.DataFrame(y, x)
df.to_csv("output", sep="\t", header=None)
