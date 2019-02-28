
#include"UserCode/OmtfAnalysis/interface/AnaGenEff.h"
#include"TEfficiency.h"
#include<iostream>

float deltaR(float eta1, float phi1, float eta2, float phi2) {
  return std::sqrt(deltaR2(eta1,phi1,eta2,phi2));
}

float deltaR(L1Obj l1, GenObj gen){
  return deltaR(l1.etaValue(), l1.phiValue(), gen.eta(), gen.phi());
}


TH1D  *hEffEtaOMTFn, *hEffEtaOMTFn_D, *hEffEtaOMTFp, *hEffEtaOMTFp_D;
TH1D  *hEffEtaAll, *hEffEtaAll_D;
TEfficiency *hEffPt, *hEffEta;
TH1D* hRatePt;

void AnaGenEff::init(TObjArray& histos)
{

  Double_t PtBins[27] = {10,13,16,19,22,25,28,31,34,37,40,43,46,49,  52,55,58,61,64, 67,70,73,76,79,82,100,200};

  hEffPt         =  new TEfficiency("hEffPt", "", 26, PtBins); histos.Add(hEffPt);
  hEffEta        =  new TEfficiency("hEffEta", "", 96, -2.4,2.4); histos.Add(hEffEta);

  hRatePt   =  new TH1D("hRate",  "hRate", 26, PtBins); histos.Add(hRatePt); 

}


void AnaGenEff::run(  const EventObj* event, const GenObjColl* muons, const L1ObjColl *l1Coll)
{

  double omin = 0.8;
  double omax = 1.24;
  
  // cout << "Event has " << endl;
  // cout << ((const std::vector<GenObj>) *muons).size()  << " generated muons" << endl;
  // cout << ((const std::vector<L1Obj> )*l1Coll).size() << " L1 muons" << endl;
  
  for (auto & gen : muons->data()){
    bool hasMatch = false;
    
    for (auto & mu : l1Coll->getL1Objs() ){
      if ( TMath::Abs(mu.etaValue()) > omax || TMath::Abs(mu.etaValue()) < omin) continue;
      if (mu.q < 12) continue;
      if (mu.type != L1Obj::OMTF_emu) continue;
      if (deltaR(mu, gen) < 0.1) { hasMatch = true; break;}
    }
    
    if (gen.pt() > 24)
      hEffEta->Fill(hasMatch, gen.eta());
    if ( TMath::Abs(gen.eta()) > omin && TMath::Abs( gen.eta()) < omax) 
      hEffPt->Fill(hasMatch, gen.pt());
    
  }

  for (auto & mu  : l1Coll->getL1Objs()){
    if (mu.q < 12) continue;
    if (mu.type != L1Obj::OMTF_emu) continue;
    hRatePt->Fill(mu.pt);
    break;
  }
  

  return;

}
