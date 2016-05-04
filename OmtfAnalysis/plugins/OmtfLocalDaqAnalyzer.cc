#include "OmtfLocalDaqAnalyzer.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <map>
#include <utility>
#include <algorithm>


#include <iostream>
#include <fstream>
#include <sstream>


#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"


#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

TH1D *hPhiDT, *hPhiCSC, *hPhiRPC, *hDeltaPhiDT, *hDeltaPhiCSC, *hDeltaPhiRPC;
TH1D *hPhiAlgoMuon, *hPhiCandMuon;
TH2D *hEtaVsPhiHit, *hEtaVsPhiMuon, *hEtaVsPhiCand;
TH2D *hEtaHitVsLayer, *hPhiHitVsLayer;
TH2D *hEtaHitVsEtaCand, *hEtaMuonVsEtaCand;
TH2D *hInputVsLayer;
TH2D *hDeltaPhiVsPattern;
TH1D *hPattern;
TH2D *hHitsME12, *hHitsME13, *hHitsME22, *hHitsME32, *hHitsMB1in, *hHitsMB1out, *hHitsMB2in, *hHitsMB2out, *hHitsMB3;
TH1D *hTimeME13, *hTimeME22, *hTimeME32, *hTimeMB1, *hTimeMB2, *hTimeMB3;
TH2D *hEtaMB2vsMB1, *hEtaRB1, *hEtaME13vsME22, *hEtaME32vsME22;
TH2D *hTimeInputDT;
TH1D *hQuality;

int eta_bit( int eta) {
  int bit = 10;
  switch (eta) {
    case (1)    : bit = 0; break;
    case (1<<1) : bit = 1; break;  
    case (1<<2) : bit = 2; break;  
    case (1<<3) : bit = 3; break;  
    case (1<<4) : bit = 4; break;  
    case (1<<5) : bit = 5; break;  
    case (1<<6) : bit = 6; break;  
    case (1<<7) : bit = 7; break;  
    case (1<<8) : bit = 8; break;  
  }
  return bit;
} 

OmtfLocalDaqAnalyzer::OmtfLocalDaqAnalyzer (const edm::ParameterSet & cfg) : theInFileName(cfg.getParameter<std::string>("inFile")) {}
void OmtfLocalDaqAnalyzer::analyze(const edm::Event&, const edm::EventSetup& es)
{
  std::cout << "ANALYSE" << std::endl;
  hPhiDT = new TH1D("hPhiDT", "hPhiDT", 145, -450.,1000.);  theHistos.Add(hPhiDT);
  hPhiCSC = new TH1D("hPhiCSC","hPhiCSC", 145, -450., 1000.); theHistos.Add(hPhiCSC);
  hPhiRPC = new TH1D("hPhiRPC","hPhiRPC", 145, -450., 1000.); theHistos.Add(hPhiRPC);
  hDeltaPhiDT = new TH1D("hDeltaPhiDT","hDeltaPhiDT", 145, -450.,1000.);  theHistos.Add(hDeltaPhiDT);
//  hDeltaPhiDT = new TH1D("hDeltaPhiDT","hDeltaPhiDT", 70, -5.,65.);  theHistos.Add(hDeltaPhiDT);
  hDeltaPhiCSC = new TH1D("hDeltaPhiCSC","hDeltaPhiCSC", 145, -450.,1000.);  theHistos.Add(hDeltaPhiCSC);
  hDeltaPhiRPC = new TH1D("hDeltaPhiRPC","hDeltaPhiRPC", 145, -450.,1000.);  theHistos.Add(hDeltaPhiRPC);
//  hDeltaPhiRPC = new TH1D("hDeltaPhiRPC","hDeltaPhiRPC", 25, 40.,65.);  theHistos.Add(hDeltaPhiRPC);

  hPhiAlgoMuon = new TH1D("hPhiAlgoMuon","hPhiAlgoMuon", 145, -450.,1000.);  theHistos.Add(hPhiAlgoMuon);
  hPhiCandMuon = new TH1D("hPhiCandMuon","hPhiCandMuon", 165, -45.,120.);  theHistos.Add(hPhiCandMuon);


  hEtaVsPhiHit  = new TH2D("hEtaVsPhiHit","hEtaVsPhiHit",14, -400., 1000., 11, -0.5, 10.5);  theHistos.Add(hEtaVsPhiHit);
  hEtaVsPhiCand = new TH2D("hEtaVsPhiCand","hEtaVsPhiCand",16, -40., 120., 51, 69.5, 120.5); theHistos.Add(hEtaVsPhiCand);

  hEtaHitVsLayer = new TH2D("hEtaHitVsLayer","hEtaHitVsLayer",18,-0.5, 17.5,  11, -0.5, 10.5); theHistos.Add(hEtaHitVsLayer);
  hPhiHitVsLayer = new TH2D("hPhiHitVsLayer","hPhiHitVsLayer",18,-0.5, 17.5,  14, -400., 1000.); theHistos.Add(hPhiHitVsLayer);
  hEtaHitVsEtaCand = new TH2D("hEtaHitVsEtaCand","hEtaHitVsEtaCand",51, 69.5, 120.5, 11, -0.5, 10.5); theHistos.Add(hEtaHitVsEtaCand);
  hEtaMuonVsEtaCand = new TH2D("hEtaMuonVsEtaCand","hEtaMuonVsEtaCand",51, 69.5, 120.5, 11, -0.5, 10.5); theHistos.Add(hEtaMuonVsEtaCand);
  
  hInputVsLayer = new TH2D("hInputVsLayer","hInputVsLayer", 18,-0.5, 17.5,  14, -0.5, 13.5); theHistos.Add(hInputVsLayer);

  hDeltaPhiVsPattern = new TH2D("hDeltaPhiVsPattern","hDeltaPhiVsPattern",80,0.,80., 30, -60.,60.); theHistos.Add(hDeltaPhiVsPattern); 
  hPattern = new TH1D("hPattern","hPattern", 80, 0., 80.); theHistos.Add(hPattern);
  hQuality  = new TH1D("hQuality","hQuality", 16, -0.5, 15.5); theHistos.Add(hQuality);
  hTimeInputDT = new TH2D("hTimeInputDT","hTimeInputDT", 15, -2.5, 12.5, 6,-0.5,5.5); theHistos.Add(hTimeInputDT);
  
  int nBinsX = 120;
  double xmin = -200.;
  double xmax = 1000.;
  int nBinsY = 120;
  double ymin = -200;
  double ymax = 1000;
  hHitsMB1in  = new TH2D("hHitsMB1in","hHitsMB1in", nBinsX, xmin, xmax, nBinsY, ymin, ymax); theHistos.Add(hHitsMB1in);
  hHitsMB1out = new TH2D("hHitsMB1out","hHitsMB1out", nBinsX, xmin, xmax, nBinsY, ymin, ymax); theHistos.Add(hHitsMB1out);
  hHitsMB2in  = new TH2D("hHitsMB2in","hHitsMB2in", nBinsX, xmin, xmax, nBinsY, ymin, ymax); theHistos.Add(hHitsMB2in);
  hHitsMB2out = new TH2D("hHitsMB2out","hHitsMB2out", nBinsX, xmin, xmax, nBinsY, ymin, ymax); theHistos.Add(hHitsMB2out);
  hHitsMB3    = new TH2D("hHitsMB3","hHitsMB3", nBinsX, xmin, xmax, nBinsY, ymin, ymax); theHistos.Add(hHitsMB3);
  hHitsME12 = new TH2D("hHitsME12","hHitsME12", nBinsX, xmin, xmax, nBinsY, ymin, ymax); theHistos.Add(hHitsME12);
  hHitsME13 = new TH2D("hHitsME13","hHitsME13", nBinsX, xmin, xmax, nBinsY, ymin, ymax); theHistos.Add(hHitsME13);
  hHitsME22 = new TH2D("hHitsME22","hHitsME22", nBinsX, xmin, xmax, nBinsY, ymin, ymax); theHistos.Add(hHitsME22);
  hHitsME32 = new TH2D("hHitsME32","hHitsME32", nBinsX, xmin, xmax, nBinsY, ymin, ymax); theHistos.Add(hHitsME32);

  nBinsX = 10; xmin = -3.5; xmax = 6.5;
  hTimeMB1  = new TH1D("hTimeMB1","hTimeMB1", nBinsX, xmin, xmax); theHistos.Add(hTimeMB1);
  hTimeMB2  = new TH1D("hTimeMB2","hTimeMB2", nBinsX, xmin, xmax); theHistos.Add(hTimeMB2);
  hTimeMB3  = new TH1D("hTimeMB3","hTimeMB3", nBinsX, xmin, xmax); theHistos.Add(hTimeMB3);
  hTimeME13 = new TH1D("hTimeME13","hTimeME13", nBinsX, xmin, xmax); theHistos.Add(hTimeME13);
  hTimeME22 = new TH1D("hTimeME22","hTimeME22", nBinsX, xmin, xmax); theHistos.Add(hTimeME22);
  hTimeME32 = new TH1D("hTimeME32","hTimeME32", nBinsX, xmin, xmax); theHistos.Add(hTimeME32);

  nBinsX = 11;
  xmin = -0.5;
  xmax = 10.5;
  nBinsY = 11;
  ymin = -0.5;
  ymax = 10.5;
  hEtaMB2vsMB1 = new  TH2D("hEtaMB2vsMB1","hEtaMB2vsMB1", nBinsX, xmin, xmax, nBinsY, ymin, ymax); theHistos.Add(hEtaMB2vsMB1);
  hEtaRB1 = new  TH2D("hEtaRB1","hEtaRB1", nBinsX, xmin, xmax, nBinsY, ymin, ymax); theHistos.Add(hEtaRB1);
  hEtaME13vsME22 = new  TH2D("hEtaME13vsME22","hEtaME13vsME22", nBinsX, xmin, xmax, nBinsY, ymin, ymax); theHistos.Add(hEtaME13vsME22);
  hEtaME32vsME22 = new  TH2D("hEtaME32vsME22","hEtaME32vsME22", nBinsX, xmin, xmax, nBinsY, ymin, ymax); theHistos.Add(hEtaME32vsME22);

  TIter iObj(&theHistos);
  while (TObject* obj = iObj()) {
    std::string s = obj->GetTitle();
    s+="_";
    s+=theInFileName;
    dynamic_cast<TH1*>(obj)->SetTitle(s.c_str());
  }


  std::ifstream inFile;
  inFile.open(theInFileName);
  if (inFile) {
      std::cout <<" File "<<theInFileName<<" opened" << std::endl;
  } else {
      std::cerr << "Unable to open file: "<<theInFileName;
      exit(1);   // call system to stop
  }

  std::string line;
  int nlines = 0;
  while (std::getline(inFile, line)) {
    nlines++;
    line.erase(0, line.find_first_not_of(" \t\r\n"));
    if (line.empty()) continue; // empty of comment line

    if (line.find("------ Event:") != std::string::npos ) {
       processLastEvent();                                        // NEXT EVENT FOUNT, preocess previous one!
       std::string snum = line.substr(line.find("Event:")+6);
       snum = snum.substr(0,snum.find(" of"));
       int number = std::atoi(snum.c_str());
       theEvent.number = number; 
    }
    if (line.find("event ") != std::string::npos ) {
       processLastEvent();                                        // NEXT EVENT FOUNT, preocess previous one!
       std::string snum = line.substr(line.find("event ")+6);
       snum = snum.substr(0,snum.find(" of"));
       int number = std::atoi(snum.c_str());
       theEvent.number = number; 
    }
    if (line.find(" AlgoHit: v") != std::string::npos ) {
       OmtfAlgoHit hit;
       //phi
       std::string sphi = line.substr(line.find("phi:")+4);
       sphi = sphi.substr(0, sphi.find(","));
       hit.phi =  std::atoi(sphi.c_str());
       //eta
       std::string seta = line.substr(line.find("eta:")+4);
       seta = seta.substr(0, seta.find(","));
       hit.eta =  std::atoi(seta.c_str());
       //layer
       std::string slayer = line.substr(line.find("layer:")+6);
       slayer= slayer.substr(0, slayer.find(","));
       hit.layer=  std::atoi(slayer.c_str());
       //
       std::string sinput = line.substr(line.find("input:")+6);
       sinput= sinput.substr(0, sinput.find(","));
       hit.input=  std::atoi(sinput.c_str());
       //bx
       std::string sbx =  line.substr(line.find("bx:")+3);
       hit.bx = std::atoi(sbx.c_str());
       theEvent.hits.push_back(hit);
    }
    if (line.find("AlgoMuon: v")  != std::string::npos ) {
      OmtfAlgoMuon muon; 
      muon.valid = true;
      //
      std::string spat = line.substr(line.find("pat:")+4);
      spat = spat.substr(0, spat.find(","));
      muon.bestPattern =  std::atoi(spat.c_str());
      //
      std::string sphi = line.substr(line.find("phi:")+4);
      sphi = sphi.substr(0, sphi.find(","));
      muon.phi =  std::atoi(sphi.c_str());
      //
      std::string shitPhi = line.substr(line.find("hitPhi:")+7);
      shitPhi = shitPhi.substr(0, shitPhi.find(","));
      muon.refHitPhi =  std::atoi(shitPhi.c_str());
      //
      std::string seta = line.substr(line.find("eta:")+4);
      seta = seta.substr(0, seta.find(","));
      muon.eta=  std::atoi(seta.c_str());
      //
      theEvent.muons.push_back(muon);
    }
    if (line.find("CandMuon: ")  != std::string::npos ) {
      OmtfGmtData::CandMuon cand; 
      std::string spt= line.substr(line.find("pt:")+3);
      spt= spt.substr(0, spt.find("charge")-1);
      cand.pt =  std::atoi(spt.c_str());
      std::string sphi = line.substr(line.find("phi:")+4);
      sphi = sphi.substr(0, sphi.find("eta")-1);
      cand.phi =  std::atoi(sphi.c_str());
      std::string seta = line.substr(line.find("eta:")+4);
      seta= seta.substr(0, seta.find("track")-1);
      cand.eta=  std::atoi(seta.c_str());
      std::string sq = line.substr(line.find("q:")+2);
      sq = sq.substr(0, sq.find("pt"));
      cand.quality=  std::atoi(sq.c_str());
      if (cand.pt > 0)theEvent.gmt.muons.push_back(cand);
    }
  }
  inFile.close();
  std::cout << " nLines: " << nlines << std::endl;

  std::string dataName = theInFileName.substr(0,theInFileName.find(".")); 
  std::string outName = "histos_"+dataName+".root"; 
  TFile f(outName.c_str(),"RECREATE");
  theHistos.Write();
  f.Close();
  std::cout <<"END"<<std::endl;
}

void OmtfLocalDaqAnalyzer::processLastEvent()
{
  if (theEvent.hits.size() >0  && theEvent.muons.size() >0 && theEvent.gmt.muons.size() >0 ) {
    const OmtfAlgoMuon & muon = theEvent.muons[0];
    const OmtfGmtData::CandMuon cand = theEvent.gmt.muons[0];
    bool debug = false;


    struct HitSpec { int phi, bx, eta; }; 
    std::vector<HitSpec> hits_MB1;
    std::vector<HitSpec> hits_MB2;
    std::vector<HitSpec> hits_MB3;

    std::vector<HitSpec> hits_RB1in;
    std::vector<HitSpec> hits_RB1out;
    std::vector<HitSpec> hits_RB2in;
    std::vector<HitSpec> hits_RB2out;
    std::vector<HitSpec> hits_RB3;

    std::vector<HitSpec> hits_RE13;
    std::vector<HitSpec> hits_RE23;
    std::vector<HitSpec> hits_RE33;

    std::vector<HitSpec> hits_ME12;
    std::vector<HitSpec> hits_ME13;
    std::vector<HitSpec> hits_ME22;
    std::vector<HitSpec> hits_ME32;

    const int bx0 = 49;

    for (auto hit : theEvent.hits) {

      bool timed = false;

      int eta_bit = ::eta_bit(hit.eta); 

      HitSpec spec  = { int(hit.phi), int(hit.bx), ::eta_bit(hit.eta) }; 

      if (hit.chamberType() == OmtfAlgoHit::rpc &&  abs(int(hit.bx)-bx0) ==0) {
        timed = true;
        hPhiRPC->Fill(hit.phi);
        hDeltaPhiRPC->Fill(muon.refHitPhi-hit.phi);
        hEtaVsPhiHit->Fill(hit.phi, eta_bit);
        if (hit.layer == 10) hits_RB1in.push_back(spec);
        if (hit.layer == 11) hits_RB1out.push_back(spec);
        if (hit.layer == 12) hits_RB2in.push_back(spec);
        if (hit.layer == 13) hits_RB2out.push_back(spec);
        if (hit.layer == 14) hits_RB3.push_back(spec);
        if (hit.layer == 15) hits_RE13.push_back(spec);
        if (hit.layer == 16) hits_RE23.push_back(spec);
        if (hit.layer == 17) hits_RE33.push_back(spec);
      }
      if (hit.chamberType() == OmtfAlgoHit::dt && abs(int(hit.bx)-bx0) == 0) {
        timed = true;
        hPhiDT->Fill(hit.phi);
        hDeltaPhiDT->Fill(muon.refHitPhi-hit.phi);
        if (hit.layer == 0) hits_MB1.push_back(spec);
        if (hit.layer == 2) hits_MB2.push_back(spec);
        if (hit.layer == 4) hits_MB3.push_back(spec);
        hTimeInputDT->Fill( hit.layer*2.5 + (int(hit.bx)-bx0), hit.input);

      }
      if (hit.chamberType() == OmtfAlgoHit::csc &&  abs(int(hit.bx)-bx0) ==0) {
        timed = true;
        hPhiCSC->Fill(hit.phi);
        hDeltaPhiCSC->Fill(muon.refHitPhi-hit.phi);
        if (hit.layer == 6) hits_ME13.push_back(spec);
        if (hit.layer == 7) hits_ME22.push_back(spec);
        if (hit.layer == 8) hits_ME32.push_back(spec);
        if (hit.layer == 9) hits_ME12.push_back(spec);
      }

      if (timed) {
        hEtaHitVsLayer->Fill(hit.layer, eta_bit);
        hPhiHitVsLayer->Fill(hit.layer, hit.phi);
        hEtaHitVsEtaCand->Fill(abs(cand.eta), eta_bit);
        hInputVsLayer->Fill(hit.layer, hit.input);
      }
    }

//   if (hits_RE13.size() == 1  && hits_ME13.size() == 1 && hits_ME13.front().first == 0) debug = true;


    hDeltaPhiVsPattern->Fill(muon.bestPattern, muon.refHitPhi-muon.phi);
    hPattern->Fill(muon.bestPattern);
    hQuality->Fill(cand.quality);
    hPhiAlgoMuon->Fill(muon.phi);
    hPhiCandMuon->Fill(cand.phi);
    hEtaVsPhiCand->Fill(cand.phi, abs(cand.eta));    
    hEtaMuonVsEtaCand->Fill( abs(cand.eta), ::eta_bit(muon.eta));

//    if (hits_MB1.size() && hits_MB1.back().phi != 10 ) debug = true;
    if (hits_RB1in.size() >= 1  && hits_MB1.size() >= 1 ) {  hHitsMB1in->Fill(  hits_RB1in.front().phi,  hits_MB1.front().phi); hTimeMB1->Fill( hits_MB1.front().bx - hits_RB1in.front().bx); }
    if (hits_RB1out.size() >= 1 && hits_MB1.size() >= 1 )   hHitsMB1out->Fill( hits_RB1out.front().phi,  hits_MB1.front().phi); 
    if (hits_RB2in.size() >= 1  && hits_MB2.size() >= 1 ) {  hHitsMB2in->Fill(  hits_RB2in.front().phi,  hits_MB2.front().phi); hTimeMB2->Fill( hits_MB2.front().bx - hits_RB2in.front().bx); }
    if (hits_RB2out.size() >= 1 && hits_MB2.size() >= 1 )   hHitsMB2out->Fill( hits_RB2out.front().phi,  hits_MB2.front().phi);
    if (hits_RB3.size() >= 1    && hits_MB3.size() >= 1 ) {    hHitsMB3->Fill(    hits_RB3.front().phi,  hits_MB3.front().phi); hTimeMB3->Fill( hits_MB3.front().bx - hits_RB3.front().bx); }

    if (hits_RE23.size() >= 1  && hits_ME12.size() >= 1 )   hHitsME12->Fill(  hits_RE23.front().phi,  hits_ME12.front().phi); 
//  if (hits_ME13.size() >= 1  && hits_RE23.size() >= 1 )   hHitsME13->Fill(  hits_RE23.front().phi,  hits_ME13.front().phi); 
//  if (hits_RE13.size() >= 1  && hits_MB1.size() >= 1 )    hHitsME13->Fill(  hits_RE13.front().phi,  hits_MB1.front().phi);
    if (hits_RE13.size() >= 1  && hits_ME13.size() >= 1 ) { hHitsME13->Fill(  hits_RE13.front().phi,  hits_ME13.front().phi);  hTimeME13->Fill( hits_ME13.front().bx-hits_RE13.front().bx); }
    if (hits_RE23.size() >= 1  && hits_ME22.size() >= 1 ) { hHitsME22->Fill(  hits_RE23.front().phi,  hits_ME22.front().phi);  hTimeME22->Fill( hits_ME22.front().bx-hits_RE23.front().bx); }
    if (hits_RE33.size() >= 1  && hits_ME32.size() >= 1 ) { hHitsME32->Fill(  hits_RE33.front().phi,  hits_ME32.front().phi);  hTimeME32->Fill( hits_ME32.front().bx-hits_RE33.front().bx); }

    if (hits_MB1.size()   && hits_MB2.size() )    { hEtaMB2vsMB1->Fill(hits_MB1.front().eta, hits_MB2.front().eta); }
    if (hits_RB1in.size() && hits_RB1out.size() ) { hEtaRB1->Fill(hits_RB1in.front().eta, hits_RB1out.front().eta); } 
    if (hits_ME13.size()  && hits_ME22.size() )       { hEtaME13vsME22->Fill(hits_ME22.front().eta, hits_ME13.front().eta); }
    if (hits_ME22.size()  && hits_ME32.size() )       { hEtaME32vsME22->Fill(hits_ME22.front().eta, hits_ME32.front().eta); }

  if (debug) std::cout << theEvent << std::endl;
  }
  theEvent = OmtfData(); // reset event data after processing
}
