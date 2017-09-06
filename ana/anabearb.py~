#!/usr/bin/env python

from __future__ import division
import os, sys, math, copy

import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
import numpy as np
from pylab import figure, axes, pie, title, show
from matplotlib import rc


#######################					
# Event class
#######################
class Event:

	def __init__(self):
		self.decayTheta = -99.
		self.decayPhi = -99.
		self.decayR = -99.

		self.totHits = 0
		self.numHitPMTs = 0
		self.hitPMTs = []
		
	def SetSummary(self,decayTheta,decayPhi,decayR,totHits,numHitPMTs):
		self.decayTheta = float(copy.deepcopy(decayTheta))
		self.decayPhi = float(copy.deepcopy(decayPhi))
		self.decayR = float(copy.deepcopy(decayR))
		self.totHits = int(copy.deepcopy(totHits))
		self.numHitPMTs = int(copy.deepcopy(numHitPMTs))
		
	def AddHitPMT(self,hit):
		self.hitPMTs.append(copy.deepcopy(hit))

#function for angle between two points
def angle(theta1,phi1,theta2,phi2):
	return np.arccos(np.sin(theta1)*np.sin(theta2)*np.cos(phi1-phi2)+np.cos(theta1)*np.cos(theta2))

#function for binsize
def binsize(array_quantity):
	liste = []
	for i in range(int(max(array_quantity))+1):
   		liste.append(i-0.3)
		liste.append(i+0.3)
	return liste

########################
# Main program
########################

events = []

# read file with data
name='ice214Bi'
with open('%s.txt' %(name), 'r') as f:
	for line in f:
		event = Event()
		splitline = line.split('\t')
		event.SetSummary(splitline[0],splitline[1],splitline[2],splitline[3],splitline[4])
		for i in range(5,len(splitline)-1):
			event.AddHitPMT(map(int,splitline[i].split(' ')))
		events.append(event)

f.closed

#read file with PMT coordinates
PMTcoord=[]
with open ('PMT_geo.txt', 'r') as f:
	for line in f:
		PMTcoord.append(line.split())
	del PMTcoord[0]
f.closed

# prepare plots by generating arrays of quantities to plot
numPMTs = []
photons = []
perPMT=[]
listangl=[]
anglPMTevent=[]
anglPMTevent1=[]
anglPMTevent2=[]
#percentage hits no hits
z = 0
o = 0

for event in events:

	# list of number of hit PMTs
	numPMTs.append(float(event.numHitPMTs))
	
	if event.numHitPMTs==0:
		z+=1
	else:
		o+=1

	# list of number of total hits
	photons.append(float(event.totHits))

	if event.hitPMTs==[]:
		pass
	else:
		# list of number of hits per PMT
		for i in range(0,len(event.hitPMTs)):
			perPMT.append(float(event.hitPMTs[i][1]))

		# list of angles between event and hit PMTs
		hitPMTco=[]
		for i in range(0,len(event.hitPMTs)):
			NrPMT=event.hitPMTs[i][0]
			hitPMTco.append(PMTcoord[NrPMT])
			for j in range(0,len(hitPMTco)):
				anglPMTevent.append(angle(float(hitPMTco[j][1]),float(hitPMTco[j][2]),float(event.decayTheta),float(event.decayPhi)))

		# list of angles between event and PMTs hit n-times
#		listPMTco=[]
#		for i in range(0,len(event.hitPMTs)):
#				if event.hitPMTs[i][1]==1:
#					NrPMT=event.hitPMTs[i][0]
#					listPMTco.append(PMTcoord[NrPMT])
#					for j in range(0,len(listPMTco)):
#						anglPMTevent1.append(angle(float(listPMTco[j][1]),float(listPMTco[j][2]),float(event.decayTheta),float(event.decayPhi)))
#				elif event.hitPMTs[i][1]==2:
#					NrPMT=event.hitPMTs[i][0]
#					listPMTco.append(PMTcoord[NrPMT])
#					for j in range(0,len(listPMTco)):
#						anglPMTevent2.append(angle(float(listPMTco[j][1]),float(listPMTco[j][2]),float(event.decayTheta),float(event.decayPhi)))

	# list of angles between hit PMTs
	if len(event.hitPMTs)>=2:
		hitPMTco=[]
		for i in range(0,len(event.hitPMTs)):
			NrPMT=event.hitPMTs[i][0]
			hitPMTco.append(PMTcoord[NrPMT])
			for j in range(0,len(hitPMTco)-1):
				for h in range(j+1,len(hitPMTco)):
					listangl.append(angle(float(hitPMTco[j][1]),float(hitPMTco[j][2]),float(hitPMTco[h][1]),float(hitPMTco[h][2])))


#with open ('numPMTsdata.txt','w') as f:
#	for i in range (0,int(max(numPMTs))+1):
#		anzahl=0
#		for j in range (0,len(numPMTs)):
#			if numPMTs[j]==i:
#				anzahl=anzahl+1
#		f.write(str(i) + "\t" + str(anzahl) + "\n")
#f.closed

# do the plots
plt.rc('text', usetex=True)
plt.rc('font', family='Roman', size=13)
plt.rc('xtick', labelsize=13)
plt.rc('ytick', labelsize=13)

# to define height and width of plot
golden_mean = (np.sqrt(5.0)-1.0)/2.0
height=20
fig=plt.figure(figsize=(height,golden_mean*height))
gs = gridspec.GridSpec(2,3)

# subfigure 1 (histogram of number of hit PMTs)
plt.subplot(gs[0])
plt.xlabel('Anzahl der getroffenen PMTs pro Event')
plt.ylabel(r'H\"aufigkeit')
plt.xlim(0,max(numPMTs)+1)
wheights=np.ones_like(numPMTs)/len(numPMTs)
h1 = plt.hist(numPMTs,binsize(numPMTs),facecolor='b',log=True,weights=wheights)

# subfigure 2 (histogram of number of photons that hit any PMT)
plt.subplot(gs[1])
plt.xlabel('Anzahl der Treffer insgesamt pro Event')
plt.ylabel(r'H\"aufigkeit')
plt.xlim(0,max(photons)+1)
wheights=np.ones_like(photons)/len(photons)
h2 = plt.hist(photons,binsize(photons),facecolor='b',log=True,weights=wheights)

# subfigure 3 (histogram of number of hits per PMT)
plt.subplot(gs[2])
plt.xlabel('Anzahl der Treffer pro PMT und Event')
plt.ylabel(r'H\"aufigkeit')
plt.xlim(0,max(perPMT)+1)
wheights=np.ones_like(perPMT)/len(perPMT)
h3 = plt.hist(perPMT,binsize(perPMT),facecolor='b',log=True,weights=wheights)

# subfigure 4 (histogram of angles between hit PMTs)
plt.subplot(gs[3])
plt.xlabel('Winkel zwischen den getroffenen PMTs [Grad]')
plt.ylabel(r'H\"aufigkeit')
plt.xlim(0,180)
wheights=np.ones_like(listangl)/len(listangl)
h4 = plt.hist(np.array(listangl)*180./np.pi,30,facecolor='b',log=True,weights=wheights)

#with open ('anglPMThist.txt','w') as f:
#	f.write(str(h4[0][6]))
#f.closed
#with open ('anglPMThisttup.txt','w') as f:
#	f.write(str(h4[0]))
#f.closed

# subfigure 5 (histogram of angles between hit PMTs and event)
plt.subplot(gs[4])
plt.xlabel('Winkel zwischen den getroffenen PMTs und dem Event [Grad]')
plt.ylabel(r'H\"aufigkeit')
plt.xlim(0,180)
wheights=np.ones_like(anglPMTevent)/len(anglPMTevent)
h5 = plt.hist(np.array(anglPMTevent)*180./np.pi,40,facecolor='b',log=True,weights=wheights)

# subfigure 6 (histogram of angles between event and PMTs hit once)
#plt.subplot(gs[5])
#plt.xlabel('angles between event and PMTs hit once')
#plt.ylabel(r'H\"aufigkeit')
#plt.xlim(0,180)
#h6 = plt.hist(anglPMTevent1,40,facecolor='b')

# subfigure 7 (histogram of angles between event and PMTs hit twice)
#plt.subplot(gs[6])
#plt.xlabel('angles between event and PMTs hit twice')
#plt.ylabel(r'H\"aufigkeit')
#plt.xlim(0,180)
#plt.xticks([0,np.pi/4,np.pi/2,3*np.pi/4,np.pi],
#          ['0',r'$\frac{1}{4}\pi$',r'$\frac{1}{2}\pi$',r'$\frac{3}{4}\pi$','$\pi$'])
#h7 = plt.hist(anglPMTevent2,40,facecolor='b')

# finish plotting
fig.savefig('%s.pdf' %(name))
print "Percentage of events when no PMT gets hit: " + str(z/(z+o)) + "\n" + "Percentage of events when one or more PMT gets hit: " + str(o/(z+o))
plt.show(block=False)
raw_input("Press any Key to continue")
plt.close()
