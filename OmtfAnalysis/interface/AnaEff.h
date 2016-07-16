#ifndef UserCode_L1RpcTriggerAnalysis_AnaEff_H
#define UserCode_L1RpcTriggerAnalysis_AnaEff_H

class TObjArray;
class TH1D;
class TGraph;

class MuonObj;
class L1ObjColl;
class L1Obj;
class EventObj;
#include <vector>
#include <map>
#include <string>
#include "UserCode/OmtfAnalysis/interface/Utilities.h"

namespace edm { class ParameterSet;}

class AnaEff {
public:
  AnaEff(const edm::ParameterSet & cfg) : debug(false) {}
  void init(TObjArray& histos);
  void run( const EventObj* event, const MuonObj* muon,
            const L1ObjColl *l1Coll);
  void resume(TObjArray& histos);
  bool debug;

private:

  RunEffMap theRunMap;
  std::map< std::string, TH1D* > theHistoMap;

};

#endif
