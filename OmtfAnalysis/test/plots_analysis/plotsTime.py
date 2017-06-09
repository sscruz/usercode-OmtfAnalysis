#!/usr/bin/python

import sys
import math
from ROOT import *

def cTimeMtfs(canvas):
  c = TCanvas("cTimeMtfs","cTimeMTFs",1200,500)
  canvas.Add(c)
  c.Divide(3)
  pad1 = c.cd(1)
  pad1.SetLogy()
  hTimeBmtf= gROOT.FindObject("hTimeBmtf")
#  hTimeBmtf.Print('All')
  hTimeBmtf.SetMinimum(0.5)
  hTimeBmtf.DrawCopy()
  pad2 = c.cd(2)
  pad2.SetLogy()
  hTimeOmtf= gROOT.FindObject("hTimeOmtf")
#  hTimeOmtf.Print('All')
  hTimeOmtf.SetMinimum(0.5)
  hTimeOmtf.DrawCopy()
  pad3 = c.cd(3)
  pad3.SetLogy()
  hTimeEmtf= gROOT.FindObject("hTimeEmtf")
#  hTimeEmtf.Print('All')
  hTimeEmtf.SetMinimum(0.5)
  hTimeEmtf.DrawCopy()
  return

def plotAll(canvas) :
  cTimeMtfs(canvas)
  return


