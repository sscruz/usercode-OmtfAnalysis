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
  TH1D *hTimeOmtf, *hTimeBmtf, *hTimeEmtf;  
}

AnaTime::AnaTime(const edm::ParameterSet& cfg)
  : debug(false), theCfg (cfg)
{}

void AnaTime::init(TObjArray& histos)
{
  hTimeBmtf = new TH1D("hTimeBmtf","hTimeBmtf",5,-2.5,2.5); histos.Add(hTimeBmtf); 
  hTimeOmtf = new TH1D("hTimeOmtf","hTimeOmtf",5,-2.5,2.5); histos.Add(hTimeOmtf); 
  hTimeEmtf = new TH1D("hTimeEmtf","hTimeEmtf",5,-2.5,2.5); histos.Add(hTimeEmtf); 
}

void AnaTime::run(const EventObj* ev, const MuonObj* muon, const L1ObjColl * l1Objs)
{
  if (!muon || !muon->isValid()) return;
  std::vector<L1Obj::TYPE> mtfs= {L1Obj::BMTF, L1Obj::OMTF, L1Obj::EMTF};
  for (const auto & mtf : mtfs) {
    std::vector<L1Obj> l1mtfs = l1Objs->selectByType(mtf);
    if (l1mtfs.size()==0) continue;
    for (const auto & l1mtf : l1mtfs) {
      double deltaR = reco::deltaR( l1mtf.etaValue(), l1mtf.phiValue(), muon->eta(), muon->phi());
      std::cout <<"TIME: "<< l1mtf<< " deltaR: "<< deltaR << std::endl;
      if (fabs(l1mtf.etaValue() - muon->eta()) > 0.2) continue;
      if (deltaR > 0.8) continue;
//    if (l1mtf.q <12) continue;
      if (l1mtf.bx !=0) std::cout <<"xxxxxx" << std::endl;
      switch (mtf) {
        case (L1Obj::BMTF) : hTimeBmtf->Fill(l1mtf.bx); break; 
        case (L1Obj::OMTF) : hTimeOmtf->Fill(l1mtf.bx); break; 
        case (L1Obj::EMTF) : hTimeEmtf->Fill(l1mtf.bx); break; 
        default: ;
      }
    }
  }

}
