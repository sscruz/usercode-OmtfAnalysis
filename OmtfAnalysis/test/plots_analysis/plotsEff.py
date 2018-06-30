#!/usr/bin/python

import sys
import math
from ROOT import *
from plotsUtils import *

def cEffEtaOMTF(canvas):
  c = TCanvas('cEffEtaOMTF','cEffEtaOMTF',1200,600)
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
  hn.SetStats(0)
  hn.DrawCopy()
  pad2 = c.cd(2)
  hp  = gROOT.FindObject('hEffEtaOMTFp')
  hp_D = gROOT.FindObject('hEffEtaOMTFp_D')
  hp.Divide(hp,hp_D,1.,1.,'B')
  hp.SetMinimum(0.)
  hp.SetMaximum(1.1)
  hp.SetXTitle("eta")
  hp.SetYTitle("efficiency")
  hp.SetStats(0)
  hp.DrawCopy()
  return

def cEffEtaAll(canvas):
  c = TCanvas('cEffEtaAll','cEffEtaAll',1200,600)
  canvas.Add(c)
  hn  = gROOT.FindObject('hEffEtaAll')
  hn_D = gROOT.FindObject('hEffEtaAll_D')
  hn.Divide(hn,hn_D,1.,1.,'B')
  hn.SetMinimum(0.)
  hn.SetMaximum(1.1)
  hn.SetXTitle("eta")
  hn.SetYTitle("efficiency") 
  hn.SetStats(0)
  hn.DrawCopy()
  one = TLine()
  one.SetLineStyle(2)
  one.SetLineColor(2)
  one.DrawLine(-2.,1.,2.,1.)
  return

def cEffEta(canvas):
  c = TCanvas('cEffEta','cEffEta',1200,600)
  canvas.Add(c)

  c.SetTickx()
  c.SetTicky()
  frame = c.DrawFrame(-2.4, 0.7, 2.4, 1.03)
  frame.SetXTitle("eta")
  frame.SetYTitle("efficiency") 
  frame.SetStats(0)

  one = TLine()
  one.SetLineStyle(2)
  one.SetLineColor(1)
  one.DrawLine(-2.,1.,2.,1.)
  
  legend = TLegend(-0.7, 0.71, 0.7, 0.79,"muon p_{T}^{reco} > 1.5*p_{T}L1, p_{T}L1=16GeV","")
  legend.SetName("lEffEta")
  canvas.Add(legend)

  colors = [2,4,4,6] 
  for index, opt in enumerate(['Bmtf','Omtf','OmtfQ4','Emtf']) :
    cut='16'
    color = colors[index]
    hn_D = gROOT.FindObject('hEff_EtaDenom'+cut)
    hn  = gROOT.FindObject('hEff_EtaCut'+cut+'_'+opt)
    hn.Divide(hn,hn_D,1.,1.,'B')
    hn.SetLineColor(color)
    hn.SetLineStyle(1)
    if (opt=='OmtfQ4') : hn.SetLineStyle(3)
    hn.DrawCopy('hist same')
#    legend.AddEntry(hn,'p_{T}L1 #geq '+cut)
    legend.AddEntry(hn,opt)

  legend.Draw()
  return

def cEffPt(canvas):
  c = TCanvas('cEffPt','cEffPt',1200,400)
  canvas.Add(c)
  c.Divide(3)

  for index, region in enumerate(['Barrel','Overlap','Endcap']):
    pad = c.cd(index+1)
    pad.SetTickx()
    pad.SetTicky()
    pad.SetLogx()
#    pad.SetLogy()

    hDenom = gROOT.FindObject('hEff_PtCutDenom_'+region[0:3])
    if (hDenom==None) :  continue
    frame = hDenom.Clone()
    frame.Reset()
    frame.SetStats(0)
    frame.SetMaximum(1.05)
    frame.SetMinimum(0.)
#    frame.SetMinimum(1.e-5)
    frame.SetXTitle("muon p_{T}")
    frame.GetXaxis().SetTitleOffset(1.4)
    frame.SetYTitle("efficiency") 
    frame.SetTitle(region)
#    frame.GetXaxis().SetRange(5,32)
    frame.GetXaxis().SetRange(5,27)
    frame.DrawCopy()

    one = TLine()
    one.SetLineStyle(2)
    one.SetLineColor(1)
#   one.DrawLine(5.,1.,490.,1.)
    one.DrawLine(5.,1.,90.,1.)

#   legend = TLegend(50, 0.05,690., 0.28,"","")
    legend = TLegend(25, 0.05, 90., 0.28,"","")
    legend.SetName("lEffPt"+region)
    canvas.Add(legend)

    colors = [2,3,4,6] 
    for index, cut in enumerate(['0','10','16','25']) :
      color = colors[index]
      hn = gROOT.FindObject('hEff_uGmtPtCut'+cut+'_'+region[0:3])
      if (hn==None) :  continue
      hn.Divide(hn,hDenom,1.,1.,'B')
      hn.SetLineColor(color)
      hn.DrawCopy('same ][ e')  
      legend.AddEntry(hn,'p_{T}L1 #geq '+cut+" GeV")

    legend.Draw()

  c.Update()
  return

def cEffRunAver(canvas):
  c = TCanvas('cEffRunAver','cEffHistory',1200,600)
#  gStyle.SetOptStat(10)
  canvas.Add(c)
  c.Divide(2)
  pad1 = c.cd(1)
  h1 = gROOT.FindObject('hEffRunAver')
  h1.SetStats(1)
  h1.DrawCopy('hist')
  pad2 = c.cd(2)
  h2 = gROOT.FindObject('hEffRunAverWeighted')
  h2.SetStats(1)
  h2.DrawCopy('hist')
  c.Update()
  return

def cEffHistory(canvas):
  c = TCanvas('cEffHistory','cEffHistory',1200,600)
  canvas.Add(c)
  gr = gROOT.FindObject('hGraphEffRun')
  gr.SetName('EffHistory')
  h = runHistoFromGraph(gr)
  fillHistoFromGraph(h,gr)
  h.SetMaximum(1.02)
  h.SetMinimum(0.5)
#  gStyle.SetOptStat(10)
  h.SetStats(0)
  h.DrawCopy()
  c.Update()
  return

def cEffDelta(canvas):
  c = TCanvas('cEffDelta','cEffDelta',1200,400)
  canvas.Add(c)
  c.Divide(3)
  pad1 = c.cd(1)
  h = gROOT.FindObject('hEffDeltaR')
  h.DrawCopy()
  pad1 = c.cd(2)
  h = gROOT.FindObject('hEffDeltaPhi')
  h.DrawCopy()
  pad1 = c.cd(3)
  h = gROOT.FindObject('hEffDeltaEta')
  h.DrawCopy()
  return

def plotAll(canvas) :
  cEffHistory(canvas)
  cEffEtaOMTF(canvas)
  cEffDelta(canvas)
  cEffEtaAll(canvas)
  cEffPt(canvas)
  cEffEta(canvas)
  cEffRunAver(canvas)
  return

