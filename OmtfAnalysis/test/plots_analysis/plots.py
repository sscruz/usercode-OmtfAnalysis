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

def cDataEmulNotAgree(canvas):
  c = TCanvas("cDataEmulNotAgree","cDataEmulNotAgree",800,800)
  canvas.Add(c)
  h= gROOT.FindObject("hDataEmulNotAgree")
  h.DrawCopy('box' )
  return

def cDataEmulCompare(canvas):
  c = TCanvas('cDataEmulCompare','cDataEmulCompare',1200,600)
  canvas.Add(c)
  c.Divide(2)
  pad1 = c.cd(1)
  pad1.SetRightMargin(0.01)
  h = gROOT.FindObject('hDataEmulCompare')
  nEvnts = h.GetEntries()
  h.Scale(1/nEvnts)
  h.SetStats(1)
  gStyle.SetOptStat(10) 
  h.DrawCopy()
  pad2 = c.cd(2)
  pad2.SetLeftMargin(0.14)
  h = gROOT.FindObject('hDataEmulCompareComb')
  h.SetStats(0)
  h.DrawCopy('text')
  return

def cDataEmulPtPhi(canvas):
  c = TCanvas('cDataEmulPtPhi','cDataEmulPtPhi',800,400)
  canvas.Add(c)
  c.Divide(2)
  pad1 = c.cd(1)
  h = gROOT.FindObject('hDataEmulPt')
  h.SetStats(0)
  h.GetXaxis().SetRange(5,32)
  h.GetYaxis().SetRange(5,32)
  h.GetXaxis().SetTitleOffset(1.4)
  h.GetYaxis().SetTitleOffset(1.4)
  h.SetXTitle("data p_{T} [GeV]");
  h.SetYTitle("emul p_{T} [GeV]");
  pad1.SetLogx() 
  pad1.SetLogy() 
  h.DrawCopy('colo')
  c.cd(2)
  h = gROOT.FindObject('hDataEmulPhi')
  h.SetStats(0)
  h.GetXaxis().SetTitleOffset(1.4)
  h.GetYaxis().SetTitleOffset(1.4)
  h.SetXTitle("data phi [GMT code]");
  h.SetYTitle("emul phi [GMT code]");
  h.DrawCopy('colo')
  return

def cDataEmulEta(canvas):
  c = TCanvas('cDataEmulEta','cDataEmulEta',800,800)
  canvas.Add(c)
  h = gROOT.FindObject('hDataEmulEta')
  h.SetStats(0)
  c.SetLeftMargin(0.15)
  h.GetXaxis().SetTitleOffset(1.4)
  h.GetYaxis().SetTitleOffset(2.3)
  h.SetXTitle("data eta [GMT code]")
  h.SetYTitle("emul eta [GMT code]")
  h.DrawCopy('text')
  return




print 'Read data from: ', fileName
gROOT.Reset()
f = TFile(fileName);
f.ls();

canvas = TObjArray()
#cEvent(canvas)
cDataEmulCompare(canvas)
cDataEmulPtPhi(canvas)
cDataEmulEta(canvas)
cDataEmulNotAgree(canvas)

raw_input('press enter to exit')

for canva in canvas :
  pictName  = "fig_png/"+canva.GetName()+".png"
  canva.Print(pictName)



