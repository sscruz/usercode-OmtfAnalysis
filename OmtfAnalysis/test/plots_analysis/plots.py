#!/usr/bin/python

import sys
import math
from ROOT import *

print "Hello ROOT"
fileName = "../omtfAnalysis.root"

def cEvent(canvas):
  c = TCanvas("cEvent","cEvent",800,800)
  canvas.Add(c)
  hEvent_BX = gROOT.FindObject("hEvent_BX")
  hEvent_BX.DrawCopy()
  return

def cDataEmulCompare(canvas,histo):
  name = 'c'+histo
  c = TCanvas(name,name,800,800)
  canvas.Add(c)
  h = gROOT.FindObject(histo)
  h.DrawCopy()
  return




print 'Read data from: ', fileName
gROOT.Reset()
f = TFile(fileName);
f.ls();

canvas = TObjArray()
cEvent(canvas)
cDataEmulCompare(canvas,"hDataEmulCompareAll")
cDataEmulCompare(canvas,"hDataEmulCompareRpc")
cDataEmulCompare(canvas,"hDataEmulCompareCsc")
cDataEmulCompare(canvas,"hDataEmulCompareRpcCsc")

raw_input('press enter to exit')

for canva in canvas :
  pictName  = "fig_png/"+canva.GetName()+".png"
  canva.Print(pictName)



