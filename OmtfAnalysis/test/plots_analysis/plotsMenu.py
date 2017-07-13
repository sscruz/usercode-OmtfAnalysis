#!/usr/bin/python

import sys
import math
from ROOT import *

def cMenu(canvas,trg,range):
  name = "cMenu"+trg+"_"+str(range[0])+'_'+str(range[1])
  c = TCanvas(name,name,1400,500)
  canvas.Add(c)
  c.SetBottomMargin(0.6)
  c.SetLeftMargin(0.05)
  c.SetRightMargin(0.01)
  h = gROOT.FindObject("hMenuAlgos"+trg)
  h.SetLineColor(4)
  h.SetFillColor(4)
  h.SetFillStyle(3003)
  h.SetStats(0)
  h.LabelsOption("v")
  h.GetYaxis().SetLabelOffset(0.005)
  if (range[0] < h.GetXaxis().GetNbins()):   
    h.GetXaxis().SetRange(range[0],min(range[1],h.GetXaxis().GetNbins()))
    h.DrawCopy()
  return

def plotAll(canvas) :
  cMenu(canvas,"L1",[1,100])
  cMenu(canvas,"L1",[101,200])
  cMenu(canvas,"L1",[201,300])
  cMenu(canvas,"L1",[301,400])
#  cMenu(canvas,"L1",[401,500])
  cMenu(canvas,"HLT",[1,100])
#  cMenu(canvas,"HLT",[101,200])
#  cMenu(canvas,"HLT",[201,300])
  return


