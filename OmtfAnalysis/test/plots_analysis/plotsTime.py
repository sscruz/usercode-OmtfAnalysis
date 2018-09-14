#!/usr/bin/python

import sys
import math
from ROOT import *

def plot1(hIn):
#  h.Print('all')
  h = hIn.Clone()
  integral = max(1.,h.Integral())
  h.Scale(1./integral)
  h.SetMaximum(1.2)
  h.SetMinimum(1./integral/1.2)
  h.SetStats(0);
  t=TLatex(); t.SetNDC(0); t.SetTextSize(0.04); t.SetTextColor(2)
  entries="#Ev: {0:1.3E}".format(integral)
  h.DrawCopy('hist e')
  t.DrawLatex(0.78, 0.9, entries)
  return

def cTimeMtfs(canvas, what1, what2, what3):
  c = TCanvas("cTimeMtfs" + what1+what2+what3, "cTimeMTFs" + what1+what2+what3, 1200, 400)
  canvas.Add(c)
  c.Divide(3)

  pad1 = c.cd(1)
  pad1.SetLogy()
  hTime1 = gROOT.FindObject("hTime" + what1).Clone("hTime" + what1+"Copy")
  plot1(hTime1)

  pad2 = c.cd(2)
  pad2.SetLogy()
  hTime2 = gROOT.FindObject("hTime"+what2)
  plot1(hTime2)
 
  pad3 = c.cd(3)
  pad3.SetLogy()
  hTime3 = gROOT.FindObject("hTime"+what3)
  plot1(hTime3)
 
  c.Update()
  return

def cTimeMtfsCorr(canvas,what=''):
  c = TCanvas("cTimeMtfsCorr"+what,"cTimeMTFsCorr"+what,1200,400)
  canvas.Add(c)
  c.Divide(3)

  pad1 = c.cd(1)
  hTimeBmtfOmtf = gROOT.FindObject("hTimeBmtfOmtf"+what)
  hTimeBmtfOmtf.SetMinimum(0.5)
  hTimeBmtfOmtf.DrawCopy('col text')

  pad2 = c.cd(2)
  hTimeOmtfEmtf= gROOT.FindObject("hTimeOmtfEmtf"+what)
  hTimeOmtfEmtf.SetMinimum(0.5)
  hTimeOmtfEmtf.DrawCopy('col text')

  pad3 = c.cd(3)
  hTimeOmtfOmtf_E= gROOT.FindObject("hTimeOmtfOmtf_E"+what)
  hTimeOmtfOmtf_E.SetMinimum(0.5)
  hTimeOmtfOmtf_E.DrawCopy('col text')
  c.Update()
  return

def cTimeTrackPt(canvas) :
  c =  TCanvas("cTimeTrackPt","cTimeTrackPt",800,800) 
  canvas.Add(c)
  c.Divide(2,2)
#  c.cd(1); hTimeOmtfTrackDPhiT.DrawCopy('box')
  c.cd(1);  hTimeOmtfTrackDRM.DrawCopy()
  c.cd(2); hTimeOmtfTrackDPhiM.DrawCopy('box')
#  c.cd(3); hTimeOmtfTrackDEtaT.DrawCopy('box')
  c.cd(4); hTimeOmtfTrackDEtaM.DrawCopy('box')
  return

def cTimeTrackBX(canvas) :
  c =  TCanvas("cTimeTrackBX","cTimeTrackBX",800,800) 
  canvas.Add(c)
  c.Divide(2,2)

  c.cd(1); hTimeOmtfTrackBXT.DrawCopy('box')
  c.cd(2); hTimeOmtfTrackBXM.DrawCopy('box')
  c.cd(3); hTimeOmtfDrTrackMuon.DrawCopy('box')
  c.cd(4)
  hTimeOmtfTrackBX1.Divide(hTimeOmtfTrackBX1,hTimeOmtfTrackBX0);
  hTimeOmtfTrackBX1.DrawCopy()
  return

def plotAll(canvas) :
  cTimeMtfsCorr(canvas)
  cTimeMtfs(canvas,'BmtfAll','OmtfAll','EmtfAll')
  cTimeMtfs(canvas,'BmtfQ','OmtfQ','EmtfQ')
  cTimeMtfs(canvas,'Bmtf','Omtf','Emtf')
  cTimeMtfs(canvas,'OmtfAll','OmtfQ','Omtf')
  cTimeMtfs(canvas,'OmtfAll_E','OmtfQ_E','Omtf_E')
#  cTimeTrackPt(canvas)
#  cTimeTrackBX(canvas)
  return


