#!/usr/bin/python

import sys
import math
from ROOT import *
from plotsUtils import *

def cEffEta(canvas):
  c = TCanvas('cEffEta','cEffEta',1200,600)
  canvas.Add(c)
  c.Divide(2)
  pad1 = c.cd(1)
  hn  = gROOT.FindObject('hEffEtaOMTFn')
  hn_D = gROOT.FindObject('hEffEtaOMTFn_D')
  hn.Divide(hn,hn_D,1.,1.,'B')
  hn.SetMinimum(0.)
  hn.SetMaximum(1.1)
  hn.SetXTitle("eta")
  hn.SetYTitle("efficiency");
  hn.DrawCopy()
  pad2 = c.cd(2)
  hp  = gROOT.FindObject('hEffEtaOMTFp')
  hp_D = gROOT.FindObject('hEffEtaOMTFp_D')
  hp.Divide(hp,hp_D,1.,1.,'B')
  hp.SetMinimum(0.)
  hp.SetMaximum(1.1)
  hp.SetXTitle("eta")
  hp.SetYTitle("efficiency")
  hp.DrawCopy()
  return

def cEffHistory(canvas):
  c = TCanvas('cEffHistory','cEffHistory',1200,600)
  canvas.Add(c)
  gr = gROOT.FindObject('hGraphEffRun')
  gr.SetName('EffHistory')
  h = runHistoFromGraph(gr)
  fillHistoFromGraph(h,gr)
  h.DrawCopy()
  return

def plotAll(canvas) :
  cEffEta(canvas)
  cEffHistory(canvas)
  return

