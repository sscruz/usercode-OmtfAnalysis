#!/usr/bin/python

import sys
import math
from ROOT import *

def cEvent(canvas):
  c = TCanvas("cEvent","cEvent",800,800)
  canvas.Add(c)
  hEvent_BX = gROOT.FindObject("hEvent_BX")
  hEvent_BX.DrawCopy()
  return

def plotAll(canvas) :
  cEvent(canvas)
  return

