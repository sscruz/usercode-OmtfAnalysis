#!/usr/bin/python

import sys
import math
from ROOT import *
import plotsDataEmul
import plotsEff
import plotsEvent
import plotsMuon

print "Hello ROOT"
fileName = "../omtfAnalysis.root"

print 'Read data from: ', fileName
gROOT.Reset()
f = TFile(fileName);
f.ls();

#--------- HERE plots

canvas = TObjArray()
plotsDataEmul.plotAll(canvas)
plotsEvent.plotAll(canvas)
plotsMuon.plotAll(canvas)
plotsEff.plotAll(canvas)

#--------- HERE plots
raw_input('press enter to exit')

for canva in canvas :
  pictName  = "fig_png/"+canva.GetName()+".png"
  canva.Print(pictName)



