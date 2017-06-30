#!/usr/bin/python

import sys
import math
from ROOT import *
import plotsDataEmul
import plotsEff
import plotsEvent
import plotsMuon
import plotsTime

print "Hello ROOT"
fileName = "../omtfAnalysis.root"

print 'Read data from: ', fileName
gROOT.Reset()
f = TFile(fileName);
f.ls();

#--------- HERE plots

canvas = TObjArray()
#plotsEvent.plotAll(canvas)
#plotsMuon.plotAll(canvas)
plotsEff.plotAll(canvas)
#plotsDataEmul.plotAll(canvas)
#plotsTime.plotAll(canvas)

#--------- HERE pause 
raw_input('press enter to exit')

#--------- HERE plots
for canva in canvas :
  canvaName =  canva.ClassName()
  if canvaName != 'TCanvas' : continue
  pictName  = "fig_png/"+canva.GetName()+".png"
  canva.Print(pictName)



