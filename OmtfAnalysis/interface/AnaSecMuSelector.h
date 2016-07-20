#ifndef UserCode_OmtfAnalysis_AnaSecMuSelector_H
#define UserCode_OmtfAnalysis_AnaSecMuSelector_H

#include "FWCore/ParameterSet/interface/ParameterSet.h"
class TObjArray;
class MuonObj;
class L1ObjColl;
class EventObj;
#include <vector>

class AnaSecMuSelector {
public:
  AnaSecMuSelector(const edm::ParameterSet& cfg);
  void init(TObjArray& histos);
  MuonObj run( const EventObj* ev, 
                       const std::vector<MuonObj> & muons, 
                       const L1ObjColl * l1Objs);
  bool debug;
private:
  edm::ParameterSet theCfgTrg, theCfgProbe;
};
#endif

