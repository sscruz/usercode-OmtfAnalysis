
#include"UserCode/OmtfAnalysis/interface/AnaGenEff.h"
#include"TEfficiency.h"

float deltaR(float eta1, float phi1, float eta2, float phi2) {
  return std::sqrt(deltaR2(eta1,phi1,eta2,phi2));
}

float deltaR(L1Obj l1, GenObj gen){
  return deltaR(l1.etaValue(), l1.phiValue(), gen.eta(), gen.phi());
}


TH1D  *hEffEtaOMTFn, *hEffEtaOMTFn_D, *hEffEtaOMTFp, *hEffEtaOMTFp_D;
TH1D  *hEffEtaAll, *hEffEtaAll_D;
TEfficiency *hEffPt, *hEffEta;
TH1D* hFakeEta, *hFakePt;

void AnaGenEff::init(TObjArray& histos)
{

  int nBins = 60;
  double omin = 0.75;
  double omax = 1.35;
  hEffEtaOMTFn   = new TH1D("hEffEtaOMTFn",  "hEffEtaOMTFn",   nBins, -omax, -omin); histos.Add(hEffEtaOMTFn);
  hEffEtaOMTFn_D = new TH1D("hEffEtaOMTFn_D","hEffEtaOMTFn_D", nBins, -omax, -omin); histos.Add(hEffEtaOMTFn_D);
  hEffEtaOMTFp   = new TH1D("hEffEtaOMTFp",  "hEffEtaOMTFp",   nBins,  omin, omax); histos.Add(hEffEtaOMTFp);
  hEffEtaOMTFp_D = new TH1D("hEffEtaOMTFp_D","hEffEtaOMTFp_D", nBins,  omin, omax); histos.Add(hEffEtaOMTFp_D);

  hEffEtaAll     =  new TH1D("hEffEtaAll",    "hEffEtaAll",   96,   -2.4, 2.4); histos.Add(hEffEtaAll); 
  hEffEtaAll_D   =  new TH1D("hEffEtaAll_D",  "hEffEtaAll_D", 96,   -2.4, 2.4); histos.Add(hEffEtaAll_D); 


  hEffPt         =  new TEfficiency("hEffPt", "", 100, 0, 200); histos.Add(hEffPt);
  hEffEta        =  new TEfficiency("hEffEta", "", 96, -2.4,2.4); histos.Add(hEffEta);

  hFakeEta   =  new TH1D("hFakeEta",  "hFakeEta", 96,   -2.4, 2.4); histos.Add(hFakeEta);
  hFakePt   =  new TH1D("hFakePt",  "hFakePt", 100,   0, 200); histos.Add(hFakePt); 

}


void AnaGenEff::run(  const EventObj* event, const GenObjColl* muons, const L1ObjColl *l1Coll)
{
  // cout << "Event has " << endl;
  // cout << ((const std::vector<GenObj>) *muons).size()  << " generated muons" << endl;
  // cout << ((const std::vector<L1Obj> )*l1Coll).size() << " L1 muons" << endl;

  for (auto & mu  : ((const std::vector<L1Obj> )*l1Coll))
    hEffEtaAll->Fill( mu.etaValue() );
  
  for (auto & gen : ((const std::vector<GenObj>) *muons)){
    bool hasMatch = false;
    for (auto & mu  : ((const std::vector<L1Obj> )*l1Coll)){
      if (deltaR(mu, gen) < 0.1) { hasMatch = true; break;}
    }
    hEffPt->Fill(hasMatch, gen.pt());
    hEffEta->Fill(hasMatch, gen.eta());
  }

  for (auto & mu  : ((const std::vector<L1Obj> )*l1Coll)){
    bool hasMatch = false; 
    for (auto & gen : ((const std::vector<GenObj>) *muons)){
      if (deltaR(mu, gen) < 0.1) { hasMatch = true; break;}
    }
    if (!hasMatch){
      hFakePt->Fill(mu.ptValue());
      hFakeEta->Fill(mu.etaValue());
    }
  }
  return;

}
