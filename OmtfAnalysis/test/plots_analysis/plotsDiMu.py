#!/usr/bin/python

import sys
import math
from ROOT import *
from plotsUtils import *

def cDiMuNumberOfMuons(canvas):
  c = TCanvas("cDiMuNumberOfMuons","cDiMuNumberOfMuons",600,600)
  canvas.Add(c)
  hE=gROOT.FindObject("hDiMuMuonNumberE")
  hD=gROOT.FindObject("hDiMuMuonNumberD")
  hE.SetLineColor(4)
  hE.SetFillColor(4)
  hE.SetFillStyle(3354)
  hE.SetStats(0)
  hE.DrawCopy('')
  hE.DrawCopy('text same')
  hD.SetLineColor(2)
  hD.SetFillColor(2)
  hD.SetFillStyle(3345)
  hD.DrawCopy('same')

  c.Update()
  return


def cDiMuDistribution(canvas):
  c = TCanvas("cDiMuDistribution","cDiMuDistribution",600,600)
  canvas.Add(c)
  hE=gROOT.FindObject("hDiMuDistributionE")
  hD=gROOT.FindObject("hDiMuDistributionD")
  hE.SetLineColor(4)
  hE.SetFillColor(4)
  hE.SetFillStyle(3354)
  hE.SetStats(0)
  hE.DrawCopy('')
  hD.SetLineColor(2)
  hD.SetFillColor(2)
  hD.SetFillStyle(3345)
  hD.DrawCopy('same')
  print 'Integral Emul: ', hE.Integral() 
  print 'Integral Data: ', hD.Integral() 

  c.Update()
  return

def cDiMuDistance(canvas):
  c = TCanvas("cDiMuDistance","cDiMuDistance",1200,400)
  canvas.Add(c)
  c.Divide(3)
  pad1 = c.cd(1)
  hE=gROOT.FindObject("hDiMuDistanceE_dR")
  hD=gROOT.FindObject("hDiMuDistanceD_dR")
  hE.SetLineColor(4)
  hE.SetFillColor(4)
  hE.SetFillStyle(3354)
  hE.SetStats(0)
  hE.DrawCopy('')
  hD.SetLineColor(2)
  hD.SetFillColor(2)
  hD.SetFillStyle(3345)
  hD.DrawCopy('same')

  pad2 = c.cd(2)
  hE_eta=gROOT.FindObject("hDiMuDistanceE_eta")
  hD_eta=gROOT.FindObject("hDiMuDistanceD_eta")
  hE_eta.SetLineColor(4)
  hE_eta.SetFillColor(4)
  hE_eta.SetFillStyle(3354)
  hE_eta.SetStats(0)
  hE_eta.DrawCopy('')
  hD_eta.SetLineColor(2)
  hD_eta.SetFillColor(2)
  hD_eta.SetFillStyle(3345)
  hD_eta.DrawCopy('same')

  pad3 = c.cd(3)
  hE_phi=gROOT.FindObject("hDiMuDistanceE_phi")
  hD_phi=gROOT.FindObject("hDiMuDistanceD_phi")
  hE_phi.SetLineColor(4)
  hE_phi.SetFillColor(4)
  hE_phi.SetFillStyle(3354)
  hE_phi.SetStats(0)
  hE_phi.DrawCopy('')
  hD_phi.SetLineColor(2)
  hD_phi.SetFillColor(2)
  hD_phi.SetFillStyle(3345)
  hD_phi.DrawCopy('same')

  c.Update()
  return

def cDiMuCloseDistrib(canvas):
  c = TCanvas("cDiMuClose","cDiMuClose",500,500)
  canvas.Add(c)
  hDiMuCloseE.SetLineColor(4)
  hDiMuCloseE.DrawCopy('box ')
  hDiMuCloseD.SetLineColor(2)
  hDiMuCloseD.DrawCopy('box same')
  c.Update()
  return



def plotAll(canvas) :
  cDiMuNumberOfMuons(canvas)
  cDiMuDistribution(canvas)
  cDiMuCloseDistrib(canvas)
  cDiMuDistance(canvas)
  return

