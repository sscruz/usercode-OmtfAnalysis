#include "UserCode/OmtfAnalysis/interface/AnaEff.h"
#include "TProfile.h"
#include "TObjArray.h"
#include "TH2F.h"
#include "TH1D.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TTree.h"
#include "TFile.h"

#include "UserCode/OmtfDataFormats/interface/EventObj.h"
#include "UserCode/OmtfDataFormats/interface/MuonObj.h"
#include "UserCode/OmtfDataFormats/interface/L1Obj.h"
#include "UserCode/OmtfDataFormats/interface/L1ObjColl.h"
#include "UserCode/OmtfAnalysis/interface/Utilities.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <cmath>
#include <vector>
#include <ostream>

namespace {

  const static unsigned int nPtCuts= 4;
  const double ptCuts[nPtCuts] ={ 0., 10., 16., 25. }; 

  std::string reg[3]={"_Bar","_Ove","_End"};

  std::map< std::string, TH1D* > hm;
  TH1D  *hEffEtaOMTFn, *hEffEtaOMTFn_D, *hEffEtaOMTFp, *hEffEtaOMTFp_D;
  TH1D  *hEffEtaAll, *hEffEtaAll_D;
  TH1D  *hEffDeltaR, *hEffDeltaEta, *hEffDeltaPhi;
  TH1D  *hEffRunAver, *hEffRunAverWeighted;

  struct BestL1Obj : public L1Obj {
    BestL1Obj() : deltaR(9999.) {}
    BestL1Obj(const L1Obj & l1, const  MuonObj *muon) : L1Obj(l1) , deltaR(9999.) {
      if (l1.isValid() && muon) deltaR = reco::deltaR( l1.etaValue(),l1.phiValue(), muon->eta(), muon->phi());
    }
    bool fired(double ptCut=0., int qMin=12, double matchinDeltaR=0.5) const {
      double epsilon = 1.e-5;
      return (q>=qMin) && (ptValue()+epsilon >= ptCut) && (deltaR < matchinDeltaR); 
    }
    double deltaR;
    friend std::ostream & operator<< (std::ostream &out, const BestL1Obj &o) {
      out << (L1Obj)o << " --> BestL1Obj : "<<o.deltaR<<" fired: "<<o.fired();
      return out;
    }

  };

}




void AnaEff::resume(TObjArray& histos)
{
  TGraphErrors * hGraphRun = new TGraphErrors();
  hGraphRun->SetName("hGraphEffRun");
  histos.Add(hGraphRun);
  std::vector<unsigned int> runs = theRunMap.runs();
  hGraphRun->Set(runs.size());
  double epsil = 1.e-6;
  for (unsigned int iPoint = 0; iPoint < runs.size(); iPoint++) {
    unsigned int run = runs[iPoint];
    RunEffMap::EffAndErr effAndErr = theRunMap.effAndErr(run);
    std::cout <<" RUN: "<<run <<" eff: "<< effAndErr 
              <<" stat: "<<theRunMap.stat(run).first<<"/"<<theRunMap.stat(run).second<<std::endl;
    hGraphRun->SetPoint(iPoint, run, effAndErr.eff());
    hGraphRun->SetPointError(iPoint, 0., effAndErr.effErr());
    hEffRunAver->Fill(effAndErr.eff()-epsil);
    hEffRunAverWeighted->Fill(effAndErr.eff()-epsil, theRunMap.stat(run).first);
  }
  std::cout << "Eff run summary: " << theRunMap.effAndErr() << std::endl;
   
}


void AnaEff::init(TObjArray& histos)
{
  int nBins = 60;
  double omin = 0.75;
  double omax = 1.35;
  hEffEtaOMTFn   = new TH1D("hEffEtaOMTFn",  "hEffEtaOMTFn",   nBins, -omax, -omin); histos.Add(hEffEtaOMTFn);
  hEffEtaOMTFn_D = new TH1D("hEffEtaOMTFn_D","hEffEtaOMTFn_D", nBins, -omax, -omin); histos.Add(hEffEtaOMTFn_D);
  hEffEtaOMTFp   = new TH1D("hEffEtaOMTFp",  "hEffEtaOMTFp",   nBins,  omin, omax); histos.Add(hEffEtaOMTFp);
  hEffEtaOMTFp_D = new TH1D("hEffEtaOMTFp_D","hEffEtaOMTFp_D", nBins,  omin, omax); histos.Add(hEffEtaOMTFp_D);

  hEffRunAver = new TH1D("hEffRunAver","hEffRunAver", 50, 0., 1.); histos.Add(hEffRunAver); hEffRunAver->Sumw2();
  hEffRunAverWeighted = new TH1D("hEffRunAverWeighted","hEffRunAverWeighted", 50, 0., 1.); histos.Add(hEffRunAverWeighted); hEffRunAverWeighted->Sumw2();

  hEffEtaAll     =  new TH1D("hEffEtaAll",    "hEffEtaAll",   96,   -2.4, 2.4); histos.Add(hEffEtaAll); 
  hEffEtaAll_D   =  new TH1D("hEffEtaAll_D",  "hEffEtaAll_D", 96,   -2.4, 2.4); histos.Add(hEffEtaAll_D); 

  hEffDeltaR     = new TH1D( "hEffDeltaR", "hEffDeltaR", 100, 0., 4.);  histos.Add(hEffDeltaR);
  hEffDeltaEta     = new TH1D( "hEffDeltaEta", "hEffDeltaEta", 100, -4., 4.);  histos.Add(hEffDeltaEta);
  hEffDeltaPhi     = new TH1D( "hEffDeltaPhi", "hEffDeltaPhi", 320, -3.2, 3.2);  histos.Add(hEffDeltaPhi);
/*
  hEfficMuPt_D = new TH1D("hEfficMuPt_D","hEfficMuPt_D", L1PtScale::nPtBins, L1PtScale::ptBins); histos.Add(hEfficMuPt_D);
  hEfficRpcNoCut_N = new TH1D("hEfficRpcNoCut_N","hEfficRpcNoCut_N", L1PtScale::nPtBins, L1PtScale::ptBins);  histos.Add(hEfficRpcNoCut_N);
  hEfficRpcPtCut_N = new TH1D("hEfficRpcPtCut_N","hEfficRpcPtCut_N", L1PtScale::nPtBins, L1PtScale::ptBins);  histos.Add(hEfficRpcPtCut_N);
*/

  std::string  base("hEff");
  const unsigned int nOpt = 5;
//  std::string opt[nOpt]={"_uGmtPtCut","_BmtfPtCut","_OmtfPtCut","_EmtfPtCut"};
  std::string opt[nOpt]={"_uGmt","_Bmtf","_Omtf","_Emtf","_OmtfQ4"};
  for (unsigned int ir=0; ir<3; ++ir) {
    std::string name=base+"_PtCutDenom"+reg[ir];
    TH1D *h= new TH1D(name.c_str(),name.c_str(), L1PtScale::nPtBins, L1PtScale::ptBins); h->Sumw2(); 
    histos.Add(h); theHistoMap[name]=h;
    //for (unsigned int iopt=0; iopt< nOpt; ++iopt) {
    for (unsigned int iopt=0; iopt< 1; ++iopt) {
    for (unsigned int icut=0; icut<nPtCuts; ++icut) {
      std::ostringstream str;
      str << base << opt[iopt]<<"PtCut"<< ptCuts[icut]<<reg[ir];
      TH1D *h= new TH1D(str.str().c_str(),str.str().c_str(), L1PtScale::nPtBins, L1PtScale::ptBins); h->Sumw2(); 
      h->SetXTitle("muon p_{T} [GeV/c]  "); h->SetYTitle("events");
      histos.Add(h); theHistoMap[str.str()]=h;
    }
    }
  }

//for (unsigned int icut=2; icut<nPtCuts; ++icut) {
  {
    unsigned int icut=2;
    std::ostringstream strEtaDenom; strEtaDenom << base << "_EtaDenom"<< ptCuts[icut];
//  TH1D *hD = new TH1D(strEtaDenom.str().c_str(),strEtaDenom.str().c_str(), L1RpcEtaScale::nEtaBins, L1RpcEtaScale::etaBins); hD->Sumw2();
    TH1D *hD = new TH1D(strEtaDenom.str().c_str(),strEtaDenom.str().c_str(), 96, -2.4, 2.4); hD->Sumw2();
    hD->SetXTitle("muon pseudorapidity"); hD->SetYTitle("events"); histos.Add(hD); theHistoMap[strEtaDenom.str()]=hD;
    for (unsigned int iopt=0; iopt <nOpt; ++iopt) {
      std::ostringstream strEtaCut  ; strEtaCut << base << "_EtaCut"<< ptCuts[icut]<<opt[iopt];
//    TH1D *hN = new TH1D(strEtaCut.str().c_str(),strEtaCut.str().c_str(), L1RpcEtaScale::nEtaBins, L1RpcEtaScale::etaBins); hN->Sumw2();
      TH1D *hN = new TH1D(strEtaCut.str().c_str(),strEtaCut.str().c_str(), 96, -2.4, 2.4); hN->Sumw2();
      hN->SetXTitle("muon pseudorapidity"); hN->SetYTitle("events"); histos.Add(hN); theHistoMap[strEtaCut.str()]=hN;
    }
  }

}

void AnaEff::run(  const EventObj* event, const MuonObj* muon, const L1ObjColl *l1Coll)
{
  if (!muon) return;
  double etaMu = muon->eta();
  double ptMu  = muon->pt();  
//  std::cout <<" MUON: " << *muon << std::endl;

  //
  // best (closest) L1Obj to muon
  //
  BestL1Obj bestOMTF, bestBMTF, bestEMTF, bestuGMT;
  std::vector<L1Obj> l1s = (l1Coll->selectByType(L1Obj::BMTF)+l1Coll->selectByType(L1Obj::EMTF)+l1Coll->selectByType(L1Obj::OMTF) +l1Coll->selectByType(L1Obj::uGMT) );
  for (auto l1 : l1s) {
    BestL1Obj cand(l1,muon);
    if (cand.q > 12) cand.q = 12;
    if (cand.type==L1Obj::BMTF && (cand.q > bestBMTF.q || (cand.q==bestBMTF.q && cand.pt>bestBMTF.pt)) && cand.deltaR < 0.5) bestBMTF = cand;
    if (cand.type==L1Obj::OMTF && (cand.q > bestOMTF.q || (cand.q==bestOMTF.q && cand.pt>bestOMTF.pt)) && cand.deltaR < 0.5) bestOMTF = cand;
    if (cand.type==L1Obj::EMTF && (cand.q > bestEMTF.q || (cand.q==bestEMTF.q && cand.pt>bestEMTF.pt)) && cand.deltaR < 0.5) bestEMTF = cand;
    if (cand.type==L1Obj::uGMT && (cand.q > bestuGMT.q || (cand.q==bestuGMT.q && cand.pt>bestuGMT.pt)) && cand.deltaR < 0.5) bestuGMT = cand;
/*
    if (cand.type==L1Obj::BMTF && cand.q >= bestBMTF.q && cand.deltaR < bestBMTF.deltaR) bestBMTF = cand;
    if (cand.type==L1Obj::OMTF && cand.q >= bestOMTF.q && cand.deltaR < bestOMTF.deltaR) bestOMTF = cand;
    if (cand.type==L1Obj::EMTF && cand.q >= bestEMTF.q && cand.deltaR < bestEMTF.deltaR) bestEMTF = cand;
    if (cand.type==L1Obj::uGMT && cand.q >= bestuGMT.q && cand.deltaR < bestuGMT.deltaR) bestuGMT = cand;
*/
  }
  if (debug && bestuGMT.isValid()) std::cout <<bestuGMT << std::endl;
  if (debug && bestBMTF.isValid()) std::cout <<bestBMTF << std::endl;
  if (debug && bestOMTF.isValid()) std::cout <<bestOMTF << std::endl;
  if (debug && bestEMTF.isValid()) std::cout <<bestEMTF << std::endl;

  //
  // control histos for bestOMTF
  //
  if (bestOMTF.isValid() && ptMu > 7. && fabs(etaMu) > 0.9 && fabs(etaMu) < 1.1) {
    hEffDeltaR->Fill(bestOMTF.deltaR); 
    hEffDeltaPhi->Fill(reco::deltaPhi(bestOMTF.phiValue(),muon->phi())); 
    hEffDeltaEta->Fill(bestOMTF.etaValue()-muon->eta()); 
  } 

  //
  // efficiency vs Eta
  //
  if (ptMu > 7.) {
    TH1D* h_N = (etaMu > 0 ) ? hEffEtaOMTFp : hEffEtaOMTFn; 
    TH1D* h_D = (etaMu > 0 ) ? hEffEtaOMTFp_D : hEffEtaOMTFn_D; 
    h_D->Fill(etaMu); 
    if (bestOMTF.fired()) h_N->Fill(etaMu);
    hEffEtaAll_D->Fill(etaMu);
    if (bestBMTF.fired() || bestEMTF.fired() || bestOMTF.fired() ) hEffEtaAll->Fill(etaMu);
  }

  //
  // OMTF efficiency history 
  //
  if ( ptMu > 7. && fabs(etaMu) < 1.24 && fabs(etaMu) > 0.83 ) theRunMap.addEvent(event->run, bestOMTF.fired()); 


  //
  // efficiency with ptCut for each region
  //
  unsigned int iregion;
  if ( fabs(etaMu) < 0.83) iregion = 0;
  else if ( fabs(etaMu) < 1.24) iregion = 1;
  else iregion = 2;


  theHistoMap["hEff_PtCutDenom"+reg[iregion]]->Fill(ptMu);
  for (unsigned int icut=0; icut < nPtCuts; icut++) { 
    double threshold = ptCuts[icut];
    if ( bestuGMT.fired(threshold) ) {
       std::ostringstream strPt;  strPt  << "hEff_uGmtPtCut"<<  ptCuts[icut]<<reg[iregion];
       theHistoMap[strPt.str()]->Fill(ptMu);
    }
  }


  double threshold = ptCuts[2];
  double ptMuPlat = 1.5*threshold; 
  if (ptMu >= ptMuPlat) {
    std::ostringstream strEtaDenom;  strEtaDenom  << "hEff_EtaDenom"<<  threshold;
    theHistoMap[strEtaDenom.str()]->Fill(muon->eta()); 

    if ( bestuGMT.fired(threshold) ) {
       std::ostringstream strEtaCut;  strEtaCut  << "hEff_EtaCut"<<  threshold <<"_uGmt";
       theHistoMap[strEtaCut.str()]->Fill(muon->eta()); 
    }
    if ( bestBMTF.fired(threshold) ) {
       std::ostringstream strEtaCut;  strEtaCut  << "hEff_EtaCut"<<  threshold <<"_Bmtf";
       theHistoMap[strEtaCut.str()]->Fill(muon->eta()); 
    }
    if ( bestOMTF.fired(threshold) ) {
       std::ostringstream strEtaCut;  strEtaCut  << "hEff_EtaCut"<<  threshold <<"_Omtf";
       theHistoMap[strEtaCut.str()]->Fill(muon->eta()); 
    }
    if ( bestEMTF.fired(threshold) ) {
       std::ostringstream strEtaCut;  strEtaCut  << "hEff_EtaCut"<<  threshold <<"_Emtf";
       theHistoMap[strEtaCut.str()]->Fill(muon->eta()); 
    }
    if ( bestOMTF.fired(threshold, 4) ) {
       std::ostringstream strEtaCut;  strEtaCut  << "hEff_EtaCut"<<  threshold <<"_OmtfQ4";
       theHistoMap[strEtaCut.str()]->Fill(muon->eta()); 
    }

    if ( fabs(muon->eta()) > 1.6 && bestuGMT.fired(threshold) && !bestEMTF.fired(threshold) ) {
    std::cout <<" Muon: " << *muon << std::endl;
    std::cout <<*l1Coll << std::endl;
    std::cout <<" BestuGMT: " << bestuGMT << std::endl;
    std::cout <<" BestEMTF: " << bestEMTF<< std::endl;
    }
  }
  
}	

