import os, sys
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

def normalize(df):
    max0=max(df.iloc[:, 0]); min0=min(df.iloc[:, 0])
    df.iloc[:, 0]=(df.iloc[:, 0]-min0)/(max0-min0)
    return df.iloc[:, 0]

name=sys.argv[1]

xps=pd.read_csv("data/"+name, sep="\t", skiprows=3).iloc[:, [0, 2]]
xps=xps.set_index(xps.columns[0])
xps.iloc[:, 0]=normalize(xps)

xpsIndex=pd.read_csv("send/"+name+"Real.txt", sep=" ", skiprows=1, header=None).iloc[:, 0]
real=pd.read_csv("send/"+name+"Real.txt", sep=" ", skiprows=1, header=None).iloc[:, 1]
pred=pd.read_csv("send/"+name+"Pred.txt", sep=" ", skiprows=1, header=None).iloc[:, 1]

ymax=max(real); ymin=min(real)
real=(real-ymin)/(ymax-ymin)
ymax=max(pred); ymin=min(pred)
pred=(pred-ymin)/(ymax-ymin)

x=list(); y=list()

for i in range(len(xpsIndex)):
    x.append(xpsIndex[i])
    down=sum(xps.index<xpsIndex[i])-1
    downV=xps.iloc[down, 0]; upV=xps.iloc[down+1, 0]
    xinter=xps.index[down+1]-xps.index[down]
    slope=(upV-downV)/xinter
    y.append(downV+slope*(xpsIndex[i]-xps.index[down]))

ymax=max(y); ymin=min(y)
y=(y-ymin)/(ymax-ymin)

df=pd.DataFrame(y, x)
pred.index=df.index; real.index=df.index
df=pd.concat([df, real, pred], axis=1)
df.columns=["xps", "real", "pred"]

plt.subplots(figsize=(8, 6))

plt.subplot(411)
plt.ylabel("XPS")
plt.plot(df.iloc[:, 0], "r")

plt.subplot(412)
plt.ylabel("REAL")
plt.plot(df.iloc[:, 1], "g")

plt.subplot(413)
plt.ylabel("PRED")
plt.plot(df.iloc[:, 2], "b")

plt.subplot(414)
plt.xlabel("Kinetic Energy")
plt.plot(df.iloc[:, 0], "r", label="xps", alpha=0.5)
plt.plot(df.iloc[:, 1], "g", label="real", alpha=0.5)
plt.plot(df.iloc[:, 2], "b", label="pred", alpha=0.5)
plt.legend()

plt.show()

