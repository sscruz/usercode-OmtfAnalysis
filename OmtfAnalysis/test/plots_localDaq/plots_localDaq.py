#!/usr/bin/python

import sys
import math
from ROOT import *

print "Hello ROOT"
board =  sys.argv[1]
run   =  sys.argv[2]
fileName = "histos_OMTF"+board+"_"+run+".root"
print 'File name is: ',fileName

def cEta(canvas):
  c = TCanvas("cEta","cEta",800,800)
  canvas.Add(c)
  c.Divide(2,2)
  c.cd(1)
  hEtaMB2vsMB1 =  gROOT.FindObject("hEtaMB2vsMB1")
  hEtaMB2vsMB1.SetStats(1)
  hEtaMB2vsMB1.GetXaxis().SetNdivisions(111)
  hEtaMB2vsMB1.GetYaxis().SetNdivisions(111)
  hEtaMB2vsMB1.DrawCopy("box")
  c.cd(2)
  hEtaRB1 =  gROOT.FindObject("hEtaRB1")
  hEtaRB1.SetStats(1)
  hEtaRB1.GetXaxis().SetNdivisions(111)
  hEtaRB1.GetYaxis().SetNdivisions(111)
  hEtaRB1.DrawCopy("box")
  c.cd(3)
  hEtaME13vsME22 =  gROOT.FindObject("hEtaME13vsME22")
  hEtaME13vsME22.GetXaxis().SetNdivisions(111)
  hEtaME13vsME22.GetYaxis().SetNdivisions(111)
  hEtaME13vsME22.SetStats(1)
  hEtaME13vsME22.DrawCopy("box")
  c.cd(4)
  hEtaME32vsME22 =  gROOT.FindObject("hEtaME32vsME22")
  hEtaME32vsME22.GetXaxis().SetNdivisions(111)
  hEtaME32vsME22.GetYaxis().SetNdivisions(111)
  hEtaME32vsME22.SetStats(1)
  hEtaME32vsME22.DrawCopy("box")
  return


def cTimeInputDT(canvas):
  c = TCanvas("cTimeInputDT","cTimeInputDT",1000,600)
  canvas.Add(c)
  hTimeInputDT = gROOT.FindObject("hTimeInputDT")
  hTimeInputDT.SetStats(0)
  hTimeInputDT.GetXaxis().SetBinLabel(1,"-2") 
  hTimeInputDT.GetXaxis().SetBinLabel(2,"-1") 
  hTimeInputDT.GetXaxis().SetBinLabel(3,"MB1: 0") 
  hTimeInputDT.GetXaxis().SetBinLabel(4," 1") 
  hTimeInputDT.GetXaxis().SetBinLabel(5," 2") 
  hTimeInputDT.GetXaxis().SetBinLabel(6,"-2") 
  hTimeInputDT.GetXaxis().SetBinLabel(7,"-1") 
  hTimeInputDT.GetXaxis().SetBinLabel(8,"MB2: 0") 
  hTimeInputDT.GetXaxis().SetBinLabel(9," 1") 
  hTimeInputDT.GetXaxis().SetBinLabel(10," 2") 
  hTimeInputDT.GetXaxis().SetBinLabel(11,"-2") 
  hTimeInputDT.GetXaxis().SetBinLabel(12,"-1") 
  hTimeInputDT.GetXaxis().SetBinLabel(13,"MB3: 0") 
  hTimeInputDT.GetXaxis().SetBinLabel(14," 1") 
  hTimeInputDT.GetXaxis().SetBinLabel(15," 2") 
#  c.SetGridy()
  hTimeInputDT.GetYaxis().SetNdivisions(106)
  hTimeInputDT.GetXaxis().SetTitle("Chamber and BX")
  hTimeInputDT.GetYaxis().SetTitle("Input")
  hTimeInputDT.DrawCopy("box")
  line = TLine()
  line.SetLineColor(kRed)
  line.SetLineStyle(3)
  line.DrawLine(2.5,0.,2.5,5.)
  line.DrawLine(7.5,0.,7.5,5.)
  return

def cEtaPhiVsLayer(canvas):
  cEtaPhiVsLayer = TCanvas("cEtaPhiVsLayer","cEtaPhiVsLayer",1000,500)
  canvas.Add(cEtaPhiVsLayer)
  cEtaPhiVsLayer.Divide(2)
  cEtaPhiVsLayer.cd(1)
  hEtaHitVsLayer = gROOT.FindObject("hEtaHitVsLayer")
  hEtaHitVsLayer.SetStats(0)
  hEtaHitVsLayer.DrawCopy("box")
  cEtaPhiVsLayer.cd(2)
  hPhiHitVsLayer = gROOT.FindObject("hPhiHitVsLayer")
  hPhiHitVsLayer.SetStats(0)
  hPhiHitVsLayer.DrawCopy("box")
  return

print 'Read data from: ', fileName

gROOT.Reset()

f = TFile(fileName);
f.ls();

cPhiHit = TCanvas("cPhiHit","cPhiHit", 1000,500);
cPhiHit.Divide(3,2);
cPhiHit.cd(1);
hPhiDT = gROOT.FindObject("hPhiDT")
hPhiDT.DrawCopy();
cPhiHit.cd(2);
hPhiRPC = gROOT.FindObject("hPhiRPC")
hPhiRPC.DrawCopy();
cPhiHit.cd(3);
hPhiCSC = gROOT.FindObject("hPhiCSC")
hPhiCSC.DrawCopy();
cPhiHit.cd(4);
hDeltaPhiDT = gROOT.FindObject("hDeltaPhiDT")
hDeltaPhiDT.DrawCopy();
cPhiHit.cd(5);
hDeltaPhiRPC = gROOT.FindObject("hDeltaPhiRPC")
hDeltaPhiRPC.DrawCopy();
cPhiHit.cd(6);
hDeltaPhiCSC = gROOT.FindObject("hDeltaPhiCSC")
hDeltaPhiCSC.DrawCopy();
cPhiHit.Print(cPhiHit.GetName()+"_"+board+".png")

cPhiMuon =  TCanvas("cPhiMuon","cPhiMuon", 1000,400);
cPhiMuon.Divide(2);
cPhiMuon.cd(1);
hPhiAlgoMuon = gROOT.FindObject("hPhiAlgoMuon")
hPhiAlgoMuon.DrawCopy()
cPhiMuon.cd(2);
hPhiCandMuon = gROOT.FindObject("hPhiCandMuon")
hPhiCandMuon.DrawCopy()
cPhiMuon.Print("cPhiMuon_"+board+".png")

cEtaVsPhi =  TCanvas("cEtaVsPhi","cEtaVsPhi", 1000,400);
cEtaVsPhi.Divide(2)
cEtaVsPhi.cd(1)
hEtaVsPhiHit =  gROOT.FindObject("hEtaVsPhiHit")
hEtaVsPhiHit.GetYaxis().SetNdivisions(11)
hEtaVsPhiHit.DrawCopy("box")
cEtaVsPhi.cd(2)
hEtaVsPhiCand =  gROOT.FindObject("hEtaVsPhiCand")
hEtaVsPhiCand.DrawCopy("box")
cEtaVsPhi.Print("cEtaVsPhi_"+board+".png")


cEtaVsEta = TCanvas("cEtaVsEta","cEtaVsEta",1000,600)
cEtaVsEta.Divide(2)
cEtaVsEta.cd(1)
hEtaHitVsEtaCand =  gROOT.FindObject("hEtaHitVsEtaCand")
hEtaHitVsEtaCand.DrawCopy("box")
cEtaVsEta.cd(2)
hEtaMuonVsEtaCand =  gROOT.FindObject("hEtaMuonVsEtaCand")
hEtaMuonVsEtaCand.DrawCopy("box")
cEtaVsEta.Print("cEtaVsEta_"+board+".png")

cInputVsLayer = TCanvas("cInputVsLayer");
hInputVsLayer =   gROOT.FindObject("hInputVsLayer")
hInputVsLayer.SetStats(0)
hInputVsLayer.DrawCopy("box")
cInputVsLayer.Print("cInputVsLayer_"+board+".png")

cDeltaPhiVsPattern = TCanvas("cDeltaPhiVsPattern","cDeltaPhiVsPattern",1000,500) 
cDeltaPhiVsPattern.Divide(2)
cDeltaPhiVsPattern.cd(1)
hDeltaPhiVsPattern = gROOT.FindObject("hDeltaPhiVsPattern")
hDeltaPhiVsPattern.DrawCopy("box")
pDeltaPhiVsPattern = cDeltaPhiVsPattern.cd(2)
hPattern =  gROOT.FindObject("hPattern")
pDeltaPhiVsPattern.SetLogy()
hPattern.SetMinimum(0.5)
hPattern.DrawCopy()
cDeltaPhiVsPattern.Print("cDeltaPhiVsPattern_"+board+".png")

cHitsMB = TCanvas("cHitsMB","cHitsMB",1000,800)
cHitsMB.Divide(3,2)
cHitsMB.cd(1)
hHitsMB1in = gROOT.FindObject("hHitsMB1in")
hHitsMB1in.SetStats(0)
hHitsMB1in.DrawCopy("box")
cHitsMB.cd(2)
hHitsMB2in = gROOT.FindObject("hHitsMB2in")
hHitsMB2in.SetStats(0)
hHitsMB2in.DrawCopy("box")
cHitsMB.cd(3)
hHitsMB3 = gROOT.FindObject("hHitsMB3")
hHitsMB3.SetStats(0)
hHitsMB3.DrawCopy("box")
cHitsMB.cd(4)
hHitsMB1out = gROOT.FindObject("hHitsMB1out")
hHitsMB1out.SetStats(0)
hHitsMB1out.DrawCopy("box")
cHitsMB.cd(5)
hHitsMB2out = gROOT.FindObject("hHitsMB2out")
hHitsMB2out.SetStats(0)
hHitsMB2out.DrawCopy("box")
cHitsMB.Print("cHitsMB_"+board+".png")

cQuality = TCanvas("cQuality","cQuality")
cQuality.SetLogy()
hQuality = gROOT.FindObject("hQuality")
hQuality.SetStats(0)
hQuality.DrawCopy()
cQuality.Print("cQuality_"+board+".png")

cTimeME = TCanvas("cTimeME","cTimeME",1000,400)
cTimeME.Divide(3)
cTimeME.cd(1)
hTimeME13 = gROOT.FindObject("hTimeME13")
hTimeME13.DrawCopy()
cTimeME.cd(2)
hTimeME22 = gROOT.FindObject("hTimeME22")
hTimeME22.DrawCopy()
cTimeME.cd(3)
hTimeME32 = gROOT.FindObject("hTimeME32")
hTimeME32.DrawCopy()
cTimeME.Print("cTimeME_"+board+".png")
cTimeMB = TCanvas("cTimeMB","cTimeMB",1000,400)
cTimeMB.Divide(3)
cTimeMB.cd(1)
hTimeMB1 = gROOT.FindObject("hTimeMB1")
hTimeMB1.DrawCopy()
cTimeMB.cd(2)
hTimeMB2 = gROOT.FindObject("hTimeMB2")
hTimeMB2.DrawCopy()
cTimeMB.cd(3)
hTimeMB3 = gROOT.FindObject("hTimeMB3")
hTimeMB3.DrawCopy()
cTimeMB.Print("cTimeMB_"+board+".png")

cHitsME = TCanvas("cHitsME","cHitsME",1200,400)
cHitsME.Divide(4)
cHitsME.cd(1)
hHitsME12 = gROOT.FindObject("hHitsME12")
hHitsME12.SetStats(0)
hHitsME12.DrawCopy("box")
cHitsME.cd(2)
hHitsME13 = gROOT.FindObject("hHitsME13")
hHitsME13.SetStats(0)
hHitsME13.DrawCopy("box")
cHitsME.cd(3)
hHitsME22 = gROOT.FindObject("hHitsME22")
hHitsME22.SetStats(0)
hHitsME22.DrawCopy("box")
cHitsME.cd(4)
hHitsME32 = gROOT.FindObject("hHitsME32")
hHitsME32.SetStats(0)
hHitsME32.DrawCopy("box")
cHitsME.Print("cHitsME_"+board+".png")

'''
fline=TF1('fline','x',0.,950.)
cHitsME = TCanvas("cHitsME","cHitsME",800,800)
cHitsME.Divide(2,2)
cHitsME.cd(1)
hHitsME12 = gROOT.FindObject("hHitsME12")
hHitsME12.SetStats(0)
hHitsME12.SetTitle("RE23 vs ME22 (patterns 0,1)")
hHitsME12.DrawCopy("box")
fline.Draw("same")
cHitsME.cd(2)
hHitsME13 = gROOT.FindObject("hHitsME13")
hHitsME13.SetStats(0)
hHitsME13.SetTitle("RE13 vs ME13 (all)")
hHitsME13.DrawCopy("box")
fline.Draw("same")
cHitsME.cd(3)
hHitsME22 = gROOT.FindObject("hHitsME22")
hHitsME22.SetStats(0)
hHitsME22.SetTitle("RE23 vs ME22 (patterns 4,5)")
hHitsME22.DrawCopy("box")
fline.Draw("same")
cHitsME.cd(4)
hHitsME32 = gROOT.FindObject("hHitsME32")
hHitsME32.SetStats(0)
hHitsME32.SetTitle("RE23 vs ME22 (all patterns)")
hHitsME32.DrawCopy("box")
#fline.Draw("same")
cHitsME.Print("cHitsME_"+board+".png")
'''

canvas = TObjArray()

cEtaPhiVsLayer(canvas)
cTimeInputDT(canvas)
cEta(canvas)

for canva in canvas :
  pictName  = canva.GetName()+'_'+board+".png"
  canva.Print(pictName)

#raw_input('press enter to exit')
