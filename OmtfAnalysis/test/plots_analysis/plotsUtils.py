#!/usr/bin/python

import sys
import math
from ROOT import *

def fillHistoFromGraph(histo, graph) :

  for iPoint in range(graph.GetN()):
    run = Double(0.)
    value = Double(0.)
    graph.GetPoint(iPoint,run, value) 
    valueErr = graph.GetErrorY(iPoint)      #  TGraph returns negative error values
    if (valueErr < 0.0) : valueErr = 0.0    #   TGraphErrors returns true errors
    bin = histo.FindBin(run);
    histo.SetBinContent(bin,value);
    histo.SetBinError(bin,valueErr);

  return histo
 

def runHistoFromGraph(graph) :
 
  maxRun = 0;
  minRun = 999999999;

  for iPoint in range(graph.GetN()):
    xTmp = Double(0.)
    yTmp = Double(0.) 
    graph.GetPoint(iPoint,xTmp,yTmp)
    aValue = int(xTmp+1.e-6);
    if (aValue < minRun) : minRun = aValue
    if (aValue > maxRun) : maxRun = aValue

  minRun = int(minRun/10)*10-10
  maxRun = int(maxRun/10)*10+10
  name = "h"+graph.GetName()
  result = TH1D(name,name,maxRun-minRun+1, minRun-0.5, maxRun+0.5)
  result.Sumw2()

  result.SetXTitle("Run number");
  result.GetXaxis().SetLabelSize(0.045);
  result.GetXaxis().SetNdivisions(510);
  result.GetXaxis().SetLabelOffset(0.006);
  result.GetYaxis().SetTitleOffset(0.9);
  result.GetYaxis().SetAxisColor(2);
  result.GetYaxis().SetLabelColor(2);
  result.GetYaxis().SetTitleColor(2);
  result.SetMarkerStyle(25);
  result.SetMarkerColor(2);

  return result


