#!/cvmfs/cms.cern.ch/slc6_amd64_gcc700/cms/cmssw/CMSSW_10_2_1/external/slc6_amd64_gcc700/bin/python

import sys
import math
import libPyROOT
from ROOT import *
import plotsDataEmul
import plotsEff
import plotsEvent
import plotsMuon
import plotsTime
import plotsMenu
import plotsSynch
import plotsSecMuSel

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
#plotsEff.plotAll(canvas)
#plotsSecMuSel.plotAll(canvas)
plotsDataEmul.plotAll(canvas)
#plotsTime.plotAll(canvas)
#plotsMenu.plotAll(canvas)
#plotsSynch.plotAll(canvas)

#--------- HERE pause 
raw_input('press enter to exit')

#--------- HERE plots
for canva in canvas :
  canvaName =  canva.ClassName()
  if canvaName != 'TCanvas' : continue
  pictName  = "fig_png/"+canva.GetName()+".png"
  canva.Print(pictName)



