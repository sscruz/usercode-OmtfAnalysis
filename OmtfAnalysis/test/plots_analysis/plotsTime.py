#!/usr/bin/python

import sys
import math
from ROOT import *

def cTimeOmtf(canvas):
  c = TCanvas("cTimeOmtf","cTimeOmtf",800,800)
  c.SetLogy()
  canvas.Add(c)
  hTimeOmtf= gROOT.FindObject("hTimeOmtf")
  hTimeOmtf.DrawCopy()
  hTimeOmtf.Print('All')
  hTimeOmtf.SetMinimum(1.e-4)
  return

def plotAll(canvas) :
  cTimeOmtf(canvas)
  return


