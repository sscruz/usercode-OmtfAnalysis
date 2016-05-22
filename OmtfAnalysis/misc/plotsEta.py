#!/usr/bin/python

import sys
import math
from ROOT import *
from array import array

print "Hello ROOT"
fileName = "../out.eta_BIG"

def code2bin(code) :
  val = abs(code)
  if val == 115 : return 8
  if val == 110 : return 7
  if val == 103 : return 6 
  if val ==  99 : return 5 
  if val ==  94 : return 4 
  if val ==  90 : return 3 
  if val ==  85 : return 2 
  if val ==  78 : return 1 
  if val ==  73 : return 0 
  return 100

def cSt(canvas,hSt):
  cName = 'c'+hSt.GetName()
  c = TCanvas(cName,cName,600,600)
  c.SetGrid(1,1)
  canvas.Add(c)
  hSt.SetStats(0)
  hSt.DrawCopy('box')
  
  return

def fixLabels (histo) :
  histo.GetYaxis().SetBinLabel( 1,"73") 
  histo.GetYaxis().SetBinLabel( 2,"78") 
  histo.GetYaxis().SetBinLabel( 3,"85") 
  histo.GetYaxis().SetBinLabel( 4,"90") 
  histo.GetYaxis().SetBinLabel( 5,"94") 
  histo.GetYaxis().SetBinLabel( 6,"98") 
  histo.GetYaxis().SetBinLabel( 7,"103") 
  histo.GetYaxis().SetBinLabel( 8,"110") 
  histo.GetYaxis().SetBinLabel( 9,"115") 
  histo.GetYaxis().SetBinLabel(10,"  ") 
  histo.GetYaxis().SetBinLabel(11,"95") 
  return


gROOT.Reset()

nx=36
xmin = 0.8
xmax = 1.7
nbounds = 9
bounds = array('d',[0.76, 0.8, 0.90037, 0.952728, 1.00313, 1.05168,  1.09844,  1.14353, 1.24, 1.3])
st13e = TH2D('st13e','st13e',  64, 0., 64., 11, -0.5, 10.5)
st22e = TH2D('st22e','st22e',  64, 0., 64., 11, -0.5, 10.5)
st32e = TH2D('st32e','st32e',  64, 0., 64., 11, -0.5, 10.5)
st12e = TH2D('st12e','st12e',  64, 0., 64., 11, -0.5, 10.5)
fixLabels(st13e)
fixLabels(st22e)
fixLabels(st32e)
fixLabels(st12e)

st13 = TH2D('st13','st13',  64, 0., 64.,  len(bounds)-1,  bounds)
st22 = TH2D('st22','st22',  64, 0., 64.,  len(bounds)-1,  bounds)
st32 = TH2D('st32','st32',  64, 0., 64.,  len(bounds)-1,  bounds)
st12 = TH2D('st12','st12',  64, 0., 64.,  len(bounds)-1,  bounds)

print 'Read data from: ', fileName
file = open(fileName,'r')
for line in file.readlines():
    words = line.split()
    station = abs( int(words[1]) )
    ring    = int(words[2])
    eta     = float(words[3])
    keyWG   = int(words[4])
    code    = int(words[5])
    print '--->>', station,'/',ring,' eta:',eta,' kw: ',keyWG,' code: ',code
    if station==1 and ring==3: st13.Fill(keyWG, abs(eta) ) 
    if station==2 and ring==2: st22.Fill(keyWG, abs(eta) ) 
    if station==3 and ring==2: st32.Fill(keyWG, abs(eta) ) 
    if station==1 and ring==2: st12.Fill(keyWG, abs(eta) ) 

    if station==1 and ring==3: st13e.Fill(keyWG, code2bin(code) ) 
    if station==2 and ring==2: st22e.Fill(keyWG, code2bin(code) ) 
    if station==3 and ring==2: st32e.Fill(keyWG, code2bin(code)) 
    if station==1 and ring==2: st12e.Fill(keyWG, code2bin(code)) 
file.close()

canvas = TObjArray()
#cEvent(canvas)
#cDataEmulCompare(canvas)
#cDataEmulPtPhiEta(canvas)
cSt(canvas,st13)
cSt(canvas,st22)
cSt(canvas,st32)
cSt(canvas,st12)
cSt(canvas,st13e)
cSt(canvas,st22e)
cSt(canvas,st32e)
cSt(canvas,st12e)

raw_input('press enter to exit')

for canva in canvas :
  pictName  = "fig_png/"+canva.GetName()+".png"
  canva.Print(pictName)



