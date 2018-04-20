#!/usr/bin/python

import sys
import math
from ROOT import *

def plot1(h):
  h.Print('all')
  integral = max(1.,h.Integral())
  h.Scale(1./integral)
  h.SetMaximum(1.2)
  h.SetMinimum(1./integral/1.2)
  h.SetStats(0);
  t=TLatex(); t.SetNDC(0); t.SetTextSize(0.04); t.SetTextColor(2)
  entries="#Ev: {0:1.3E}".format(integral)
  h.DrawCopy('hist e')
  t.DrawLatex(0.8, 1.3, entries)
  return

def cTimeMtfs(canvas, what=''):
  c = TCanvas("cTimeMtfs" + what, "cTimeMTFs" + what, 1200, 500)
  canvas.Add(c)
  c.Divide(3)

  pad1 = c.cd(1)
  pad1.SetLogy()
  hTimeBmtf = gROOT.FindObject ("hTimeBmtf" + what)
  plot1(hTimeBmtf)

  pad2 = c.cd(2)
  pad2.SetLogy()
  hTimeOmtf= gROOT.FindObject("hTimeOmtf"+what)
  plot1(hTimeOmtf)
 
  pad3 = c.cd(3)
  pad3.SetLogy()
  hTimeEmtf= gROOT.FindObject("hTimeEmtf"+what)
  plot1(hTimeEmtf)
 
  return

def cTimeMtfsCorr(canvas,what=''):
  c = TCanvas("cTimeMtfsCorr"+what,"cTimeMTFsCorr"+what,1200,600)
  canvas.Add(c)
  c.Divide(2)

  pad1 = c.cd(1)
  hTimeBmtfOmtf = gROOT.FindObject("hTimeBmtfOmtf"+what)
  hTimeBmtfOmtf.SetMinimum(0.5)
  hTimeBmtfOmtf.DrawCopy('col text')

  pad2 = c.cd(2)
  hTimeOmtfEmtf= gROOT.FindObject("hTimeOmtfEmtf"+what)
  hTimeOmtfEmtf.SetMinimum(0.5)
  hTimeOmtfEmtf.DrawCopy('col text')
  c.Update()
  return

def plotAll(canvas) :
  cTimeMtfsCorr(canvas)
#  cTimeMtfs(canvas)
#  cTimeMtfs(canvas,'Q')
  cTimeMtfs(canvas,'All')
  return


