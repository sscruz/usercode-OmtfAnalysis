#ifndef UserCode_L1RpcTriggerAnalysis_AnaDiMu_H
#define UserCode_L1RpcTriggerAnalysis_AnaDiMu_H

class TObjArray;
class TH1D;

class MuonObj;
class MuonObjColl;
class L1ObjColl;
class L1Obj;
class EventObj;

#include <vector>
#include <map>
#include <string>
#include "UserCode/OmtfAnalysis/interface/Utilities.h"

namespace edm { class ParameterSet;}

class AnaDiMu {
public:
  AnaDiMu(const edm::ParameterSet & cfg) : debug(false) {}
  void init(TObjArray& histos);
  void run( const EventObj* event, 
            const MuonObjColl* muons,
            const L1ObjColl *l1Coll);
  bool debug;

private:

};

#endif
