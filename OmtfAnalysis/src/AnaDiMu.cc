#include "UserCode/OmtfAnalysis/interface/AnaDiMu.h"
#include "TProfile.h"
#include "TObjArray.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TTree.h"
#include "TFile.h"

#include "UserCode/OmtfDataFormats/interface/EventObj.h"
#include "UserCode/OmtfDataFormats/interface/MuonObj.h"
#include "UserCode/OmtfDataFormats/interface/MuonObjColl.h"
#include "UserCode/OmtfDataFormats/interface/L1Obj.h"
#include "UserCode/OmtfDataFormats/interface/L1ObjColl.h"
#include "UserCode/OmtfAnalysis/interface/Utilities.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <cmath>
#include <vector>
#include <ostream>

namespace {
  TH1D* hDiMuMuonNumberD;
  TH1D* hDiMuMuonNumberE;
  TH1D* hDiMuDistributionD;
  TH1D* hDiMuDistributionE;
  TH1D* hDiMuDistanceD_dR;
  TH1D* hDiMuDistanceD_eta;
  TH1D* hDiMuDistanceD_phi;
  TH1D* hDiMuDistanceE_dR;
  TH1D* hDiMuDistanceE_eta;
  TH1D* hDiMuDistanceE_phi;
  TH2D* hDiMuCloseD;
  TH2D* hDiMuCloseE;
}


void AnaDiMu::init(TObjArray& histos)
{
  hDiMuMuonNumberD = new TH1D("hDiMuMuonNumberD","hDiMuMuonNumber",10,0.,10.); histos.Add(hDiMuMuonNumberD);
  hDiMuDistributionD = new TH1D("hDiMuDistributionD","hDiMuDistribution",48,-2.4,2.4); histos.Add(hDiMuDistributionD);
  hDiMuDistanceD_dR = new TH1D("hDiMuDistanceD_dR","hDiMuDistance_dR",100,0.,2.); histos.Add(hDiMuDistanceD_dR);
  hDiMuDistanceD_eta = new TH1D("hDiMuDistanceD_eta","hDiMuDistance_eta",100,0.,2.); histos.Add(hDiMuDistanceD_eta);
  hDiMuDistanceD_phi = new TH1D("hDiMuDistanceD_phi","hDiMuDistance_phi",100,0.,2.); histos.Add(hDiMuDistanceD_phi);
  hDiMuCloseD = new TH2D("hDiMuCloseD","hDiMuClose",96,-2.4,2.4, 72, 0.,2*M_PI); histos.Add(hDiMuCloseD);

  hDiMuMuonNumberE = new TH1D("hDiMuMuonNumberE","hDiMuMuonNumber",10,0.,10.); histos.Add(hDiMuMuonNumberE);
  hDiMuDistributionE = new TH1D("hDiMuDistributionE","hDiMuDistribution",48,-2.4,2.4); histos.Add(hDiMuDistributionE);
  hDiMuDistanceE_dR  = new TH1D("hDiMuDistanceE_dR","hDiMuDistance_dR",20,0.,1.); histos.Add(hDiMuDistanceE_dR);
  hDiMuDistanceE_eta = new TH1D("hDiMuDistanceE_eta","hDiMuDistance_eta",20,0.,1.); histos.Add(hDiMuDistanceE_eta);
  hDiMuDistanceE_phi = new TH1D("hDiMuDistanceE_phi","hDiMuDistance_phi",20,0.,1.); histos.Add(hDiMuDistanceE_phi);

  hDiMuCloseE = new TH2D("hDiMuCloseE","hDiMuClose",96,-2.4,2.4, 72, 0.,2*M_PI); histos.Add(hDiMuCloseE);
}

void AnaDiMu::run(  const EventObj* event, const MuonObjColl* muons, const L1ObjColl *l1Coll)
{
  std::vector<L1Obj> vEmul = l1Coll->selectByType(L1Obj::uGMT_emu).selectByBx(0,0).selectByQuality(8).getL1Objs();
  std::vector<L1Obj> vData = l1Coll->selectByType(L1Obj::uGMT).selectByBx(0,0).selectByQuality(8).getL1Objs();

  hDiMuMuonNumberD->Fill(vData.size());
  hDiMuMuonNumberE->Fill(vEmul.size());
  
  if ( vData.size()==2) {
    for (const auto & mu : vData) {hDiMuDistributionD->Fill(mu.etaValue());}
    double deltaR = reco::deltaR( vData[0].etaValue(), vData[0].phiValue(), vData[1].etaValue(), vData[1].phiValue());
    hDiMuDistanceD_dR->Fill(deltaR);
    hDiMuDistanceD_eta->Fill(vData[1].etaValue()-vData[0].etaValue());
    hDiMuDistanceD_phi->Fill(reco::deltaPhi(vData[1].phiValue(), vData[0].phiValue()));
    if (deltaR < 0.25) hDiMuCloseD->Fill(vData[0].etaValue(), vData[0].phiValue());
  }
  if ( vEmul.size()==2) {
    for (const auto & mu : vEmul) {hDiMuDistributionE->Fill(mu.etaValue());}
    double deltaR = reco::deltaR( vEmul[0].etaValue(), vEmul[0].phiValue(), vEmul[1].etaValue(), vEmul[1].phiValue());
    hDiMuDistanceE_dR->Fill(deltaR);
    hDiMuDistanceE_eta->Fill(vEmul[1].etaValue()-vEmul[0].etaValue());
    hDiMuDistanceE_phi->Fill(reco::deltaPhi(vEmul[1].phiValue(), vEmul[0].phiValue()));
    if (deltaR < 0.25) hDiMuCloseE->Fill(vEmul[0].etaValue(), vEmul[0].phiValue());
    if (deltaR < 0.25) {
//  if (vData.size()==1) std::cout<< *event <<" Delta R: " << deltaR <<std::endl <<*muons<<std::endl<<l1Coll->selectByBx(0,0)<<std::endl;
    }
  }
 
 
  
   
 
}	

