#! /usr/bin/env python

import numpy as np
from matplotlib import pyplot as plt
from scipy import fftpack as fft

p=[0.5,1.3,2.25,2.3,2.85,2.9]
fname=[]

for n in np.arange(0.5,5.5,0.1):
  fname.append("timeSeries_%s" % n)

fname.remove("timeSeries_1.0")
fname.remove("timeSeries_2.0")
fname.remove("timeSeries_3.0")
fname.remove("timeSeries_4.0")
fname.remove("timeSeries_5.0")

for n in range(1,6): 
 fname.append("timeSeries_%s" % n)
for name in fname:
  f=open(name+".dat")
  data=[]
  for line in f:
    data.append(float(line.split('\n')[0]))
  data=np.array(data)
  tf=abs(fft.rfft(data))
  tf=tf/np.max(tf) #normalizo
  x=np.linspace(1,len(tf)/2,len(tf)/2)/(0.001*len(tf))
  
  fig=plt.figure(figsize=(10.24,7.68))#(40.96,21.60))
  graph=fig.add_subplot(111)
  graph.xaxis.set_major_locator(plt.MaxNLocator(10))
  
  graph.plot(x[:500],tf[:500])

  plt.draw()
  plt.savefig(name+"FFT.eps")
  
  fig=plt.figure(figsize=(10.24,7.68))#(40.96,21.60))
  graph=fig.add_subplot(111)
  graph.xaxis.set_major_locator(plt.MaxNLocator(10))
  x=np.linspace(0,len(data),len(data))
  graph.plot(x*0.001,data)
  plt.draw()
  plt.savefig(name+".eps")
  
  f2=open(name+"_2d.dat")
  data2=[]
  #data=[]
  for line2 in f2:
    data2.append(float(line2.split()[1]))
    #data.append(float(line2.split()[0]))
  fig=plt.figure(figsize=(10.24,7.68))#(40.96,21.60))
  graph=fig.add_subplot(111)
  graph.xaxis.set_major_locator(plt.MaxNLocator(10))
  graph.plot(data,data2)
  plt.draw()
  plt.savefig(name+"_2d.eps")

  del(data,f,graph,fig,line,data2)

  
