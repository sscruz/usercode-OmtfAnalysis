#!/usr/bin/python

import sys
import math
from ROOT import *

def cSynch_delaySummary(canvas):
  c = TCanvas("cSynch_delaySummary","cSynch_delaySummary",600,600)
  canvas.Add(c)
  h = gROOT.FindObject("hSynch_delaySummary")
  integral = h.Integral()
  h.SetTitle("RPC hit synchronization;Chamber hit timing wrt L1A [BX];Hits / bin")
# h.Sumw2()
  h.Scale(1.0/integral)
  h.SetYTitle("Fraction of hits")
  h.GetXaxis().SetRange(1,7)


  h.SetTitleSize(0.045,"Y")
  h.SetTitleSize(0.045,"X")
  h.SetTitleOffset(1.5,"Y")
  h.SetTitleOffset(1.2,"X")

  h.SetMarkerSize(1.7)  # for TEXT bin values
  h.SetMarkerColor(4);  # for TEXT bin valued
  h.SetStats(0);

  meanX = h.GetMean()
  rmsX = h.GetRMS()

  h.SetLineStyle(1)
  h.SetLineWidth(2)
  h.SetMarkerStyle(1)
  h.SetLineColor(4)
  h.SetFillColor(4)
  h.SetFillStyle(3003)
  h.GetXaxis().SetNdivisions(10)
  h.DrawCopy("E1 E0")
  h.DrawCopy("HIST SAME")

  c.SetLeftMargin(0.17);
  c.SetBottomMargin(0.15);
  c.SetGrid(0,1);
  c.SetRightMargin(0.02);
  c.SetLogy();

  xpos=0.20
  ypos=0.85
  legendFontSize = 0.030
  legendOffset   = 0.030
  t=TLatex()
  t.SetNDC(1)
  t.SetTextSize(legendFontSize)
  t.SetTextColor(2)
  aver ="Avg time = {0:+.4f} BX".format(meanX)
  t.DrawLatex(xpos, ypos, aver)
  rms = "Rms = {0:.4f} BX".format(rmsX) 
  t.DrawLatex(xpos, ypos-legendOffset, rms)

  c.Update()
  return

def cSynch_delaySpread(canvas):
  c = TCanvas("cSynch_delaySpread","cSynch_delaySpread",600,600)
  canvas.Add(c)
  h = gROOT.FindObject("hSynch_delaySpread")

  nLB_total = h.Integral()-1;
  nLB_dead = h.GetBinContent(1,1) -1;
#  gStyle.SetPalette(1);
  c.SetLogz();
  c.SetGrid(1,1);
  c.SetRightMargin(0.2);
  c.SetLeftMargin(0.17);
  c.SetBottomMargin(0.15);
  c.Draw();

  h.SetTitleSize(0.045,"Y");
  h.SetTitleSize(0.045,"X");
  h.SetTitleSize(0.045,"Z");
  h.SetLabelSize(0.038,"X");
  h.SetLabelSize(0.038,"Y");
  h.SetLabelSize(0.038,"Z");
  h.SetTitleOffset(1.5,"Y");
  h.SetTitleOffset(1.2,"X");
  h.SetTitleOffset(1.2,"Z");
  h.SetMaximum(nLB_total);
  h.GetXaxis().CenterTitle(1);
  h.SetMarkerSize(1.0);  # for TEXT bin values
  h.SetMarkerColor(1);   # for TEXT bin valued
  h.SetLineStyle(1);
  h.SetMarkerStyle(1);
  h.SetStats(False);

  h.GetXaxis().SetRangeUser(-0.12, 0.12);
#  h.GetXaxis().SetRangeUser(-3. , 3.);
  h.GetYaxis().SetRangeUser(0.0, 0.45);
#  h.GetYaxis().SetRangeUser(0.0, 3.45);

  h.GetXaxis().SetNdivisions(410);
  h.GetYaxis().SetNdivisions(410);
  h.DrawCopy("COLZ");
  h.DrawCopy("TEXT0,SAME");

  xpos=0.20
  ypos=0.85
  legendFontSize = 0.030
  legendOffset   = 0.030
  t=TLatex()
  t.SetNDC(1)
  t.SetTextSize(legendFontSize)
  t.SetTextColor(2)
  t.DrawLatex(xpos, ypos, "Number of Link Boards:")
  t.DrawLatex(xpos, ypos-legendOffset, "{0} - total".format(nLB_total) )
  t.DrawLatex(xpos, ypos-2*legendOffset, "{0} - w/o hits".format(nLB_dead))
  c.Update()

  return 

def cSynch_top10(canvas, what):
  c = TCanvas("cSynch_top10_"+what,"cSynch_top10_"+what,600,600)
  canvas.Add(c)
  h = gROOT.FindObject("hSynch_top"+what)
  PEAK_BIN = 3;
  for nbx in range(0,h.GetNbinsX()):
    label="{0:d}".format(nbx-PEAK_BIN)
    h.GetXaxis().SetBinLabel(nbx+1,label)
   
  h.SetTitle( "Top 10 " + what + " LBs" );
  h.GetXaxis().SetTitle("Chamber hit timing wrt L1 [BX]");
  h.GetXaxis().SetRange(1,7);
  c.SetLeftMargin(0.4);
  h.SetStats(0);
  h.SetFillStyle(0);
  h.SetMarkerSize(1.5);
  h.SetMarkerColor(4);
  h.SetTitleSize(0.04,"X");
  h.SetTitleOffset(1.05,"X");
  h.LabelsOption("h","X");

  h.DrawCopy("TEXT0");
  c.Update()
  return

def cSynch_NotComplete(canvas):
  c = TCanvas("cSynch_NotComplete","cSynch_NotComplete",1200,300)
  canvas.Add(c)
  c.Divide(3,1);
  for fed in range(790,793):
    pad = c.cd(fed-789);
    pad.SetTopMargin(0.1);
    pad.SetGrid(1,1);
    h = gROOT.FindObject( "hSynch_notComplete{0:3d}".format(fed) );
    h.GetYaxis().SetNdivisions(510);
    h.GetYaxis().SetNdivisions(605);
    h.GetXaxis().SetTitleOffset(1.0);
    h.GetYaxis().SetTitleOffset(1.0);
    h.DrawCopy("box");
    fedStr = "Fed {0}".format(fed)
    t=TText(); 
    t.SetTextColor(4); t.SetTextAlign(11); t.SetTextSize(0.06);
    t.DrawTextNDC(0.16,0.92,fedStr);

  c.Update()
  return


def plotAll(canvas) :
  cSynch_delaySummary(canvas)
  cSynch_delaySpread(canvas)
  cSynch_top10(canvas,"Spread")
  cSynch_top10(canvas,"Occup")
  cSynch_NotComplete(canvas)
  return


