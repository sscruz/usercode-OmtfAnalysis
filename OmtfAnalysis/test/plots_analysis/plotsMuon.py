#!/usr/bin/python

import sys
import math
from ROOT import *

def cMuonDIS(canvas):
  c = TCanvas('cMuonDIS','cMuonDIS',1200,600)
  canvas.Add(c)
  c.Divide(3)
  pad1 = c.cd(1)
  pad1.SetLogy()
  h = gROOT.FindObject('hMuonPt_DIS')
  h.SetStats(0)
  h.SetMinimum(0.9)
  h.DrawCopy()
  hM = gROOT.FindObject('hMuonPt_MEN')
  hM.SetLineColor(3)
  hM.DrawCopy('same')
  pad2 = c.cd(2)
  h = gROOT.FindObject('hMuonEta_DIS')
  h.SetMinimum(0.)
  h.SetStats(0)
  h.DrawCopy()
  hM = gROOT.FindObject('hMuonEta_MEN')
  hM.SetLineColor(3)
  hM.DrawCopy('same')
  pad3 = c.cd(3)
  h = gROOT.FindObject('hMuonPhi_DIS')
  h.SetMinimum(0.)
  h.SetStats(0)
  h.DrawCopy()
  hM = gROOT.FindObject('hMuonPhi_MEN')
  hM.SetLineColor(3)
  hM.DrawCopy('same')
  return
def cMuonPtVsEta(canvas):
  c = TCanvas('cMuonPtVsEta','cMuonPtVsEta',1200,600)
  canvas.Add(c)
  c.Divide(3)
  pad1 = c.cd(1)
  h = gROOT.FindObject('hMuonPtVsEta_Gl')
  h.SetStats(0)
  h.SetMinimum(0.)
  h.DrawCopy("box")
  pad2 = c.cd(2)
  h = gROOT.FindObject('hMuonPtVsEta_Ma')
  h.SetMinimum(0.)
  h.SetStats(0)
  h.DrawCopy("box")
  pad3 = c.cd(3)
  h = gROOT.FindObject('hMuonPtVsEta_Tk')
  h.SetMinimum(0.)
  h.SetStats(0)
  h.DrawCopy("box")
  return

def plotAll(canvas) :
  cMuonDIS(canvas)
  cMuonPtVsEta(canvas)
  return

