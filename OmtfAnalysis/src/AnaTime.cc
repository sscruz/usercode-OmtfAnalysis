#include "UserCode/OmtfAnalysis/interface/AnaTime.h"

#include "TObjArray.h"
#include "TH1D.h"
#include "TH2D.h"
#include "UserCode/OmtfDataFormats/interface/MuonObj.h"
#include "UserCode/OmtfDataFormats/interface/EventObj.h"
#include "UserCode/OmtfDataFormats/interface/L1ObjColl.h"
#include "UserCode/OmtfAnalysis/interface/Utilities.h"
#include "DataFormats/Math/interface/deltaR.h"


#include <ostream>
#include <iostream>
#include <cmath>

namespace { 
  TH1D *hTimeOmtf;  
}

AnaTime::AnaTime(const edm::ParameterSet& cfg)
  : debug(false), theCfg (cfg)
{}

void AnaTime::init(TObjArray& histos)
{
  hTimeOmtf = new TH1D("hTimeOmtf","hTimeOmtf",5,-2.5,2.5); histos.Add(hTimeOmtf); 
}

void AnaTime::run(const EventObj* ev, const MuonObj* muon, const L1ObjColl * l1Objs)
{
  if (!muon || !muon->isValid()) return;
  std::vector<L1Obj> omtfs = l1Objs->selectByType(L1Obj::OMTF);
  if (omtfs.size()==0) return;
  std::cout <<"HERE TIME"<< std::endl;
  for (const auto & omtf : omtfs) {
    double deltaR = reco::deltaR( omtf.etaValue(), omtf.phiValue(), muon->eta(), muon->phi());
    std::cout <<"TIME: "<< omtf<< " deltaR: "<< deltaR << std::endl;
    if (fabs(omtf.etaValue() - muon->eta()) > 0.2) continue;
    if (deltaR > 0.8) continue;
    hTimeOmtf->Fill(omtf.bx); 
  }

}
