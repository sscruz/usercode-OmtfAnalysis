#!/usr/bin/python

import sys
import math
from ROOT import *
from plotsUtils import *

def cSecMuSelPtDis(canvas) :
  c = TCanvas('cSecMuSelPtDis','cSecMuSelPtDis',600,600)
  canvas.Add(c)
#  c.SetLogx()
#  c.SetLogy()
  c.SetRightMargin(0.14);
  c.SetLeftMargin(0.11);

  h = gROOT.FindObject('hSecMuPtDis')
  h.SetXTitle("trg p_{T}   [GeV]")
  h.SetYTitle("probe p_{T}  [GeV]")
  h.GetYaxis().SetTitleOffset(1.4);
  h.SetStats(0)
  h.DrawCopy('colz')
  c.Update()
  return

def cSecMuSelEtaDis(canvas) :
  c = TCanvas('cSecMuSelEtaDis','cSecMuSelEtaDis',600,600)
  canvas.Add(c)
  c.SetLogx()
  c.SetLogy()
  h = gROOT.FindObject('hSecMuEtaDis')
  h.SetXTitle('trg~{\eta}')
  h.SetYTitle("probe~ \eta")
  h.SetStats(0)
  h.DrawCopy('colz')
  c.Update()
  return

def cSecMuSelDiff(canvas) :
  c = TCanvas('cSecMuSelDiff','cSecMuSelDiff',1200,400)
  canvas.Add(c)
  c.Divide(3)
  p1 = c.cd(1)
  p1.SetLogy()
  h = gROOT.FindObject('hSecMuDeltaR')
  h.SetXTitle("Delta R")
  h.SetYTitle(" #ev")
  h.SetStats(0)
  h.DrawCopy()
  p2 = c.cd(2)
  p2.SetLogy()
  h = gROOT.FindObject('hSecMuDeltaPhi')
  h.SetXTitle("Delta Phi")
  h.SetYTitle(" #ev")
  h.SetStats(0)
  h.DrawCopy()
  p3 = c.cd(3)
  p3.SetLogy()
  h = gROOT.FindObject('hSecMuDeltaEta')
  h.SetXTitle("\{Delta} Eta")
  h.SetYTitle(" #ev")
  h.SetStats(0)
  h.DrawCopy()

  c.Update()
  return

def plotAll(canvas) :
  cSecMuSelPtDis(canvas)
  cSecMuSelEtaDis(canvas)
  cSecMuSelDiff(canvas)
  return
