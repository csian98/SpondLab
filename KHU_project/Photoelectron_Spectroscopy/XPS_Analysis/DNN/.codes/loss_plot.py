#!/opt/homebrew/Caskroom/miniforge/base/envs/tf_38/bin/python

import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter

fp=open("./loss1.txt", "r")
loss1=list(map(float, fp.read()[1:-1].split(',')))
fp.close()
fp=open("./loss2.txt", "r")
loss2=list(map(float, fp.read()[1:-1].split(',')))
fp.close()

if __name__=="__main__":
  min1=loss1.index(min(loss1))
  min2=loss2.index(min(loss2))

  fig=plt.figure()
  fig.set_size_inches(10, 4)
  fig.suptitle("Losses")

  axl=fig.add_subplot(1, 2, 1)
  axl.plot(loss1, 'o-', color="#00a3d2")
  axl.axvline(min1, color="#881824", alpha=0.3)
  axl.axhline(loss1[min1], color="#881824", alpha=0.3)
  axl.text(min1, loss1[min1], "epoch=%s"%min1)
  axl.set_xlabel("#Epoch")
  #axl.set_yscale("log")
  axl.yaxis.set_major_formatter(FormatStrFormatter("%.1e"))

  axl=fig.add_subplot(1, 2, 2)
  axl.plot(loss2, 'o-', color="#00a3d2")
  axl.axvline(min2, color="#881824", alpha=0.3)
  axl.axhline(loss2[min2], color="#881824", alpha=0.3)
  axl.text(min2, loss2[min2], "epoch=%s"%min2)
  axl.set_xlabel("#Epoch")
  #axl.set_yscale("log")
  axl.yaxis.set_major_formatter(FormatStrFormatter("%.1e"))

  plt.show()
