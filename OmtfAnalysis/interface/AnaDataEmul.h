#ifndef UserCode_OmtfAnalysis_AnaDataEmul_H
#define UserCode_OmtfAnalysis_AnaDataEmul_H

#include <string>
#include "UserCode/OmtfAnalysis/interface/Utilities.h"

class L1ObjColl;
class L1Obj;
class EventObj;
class TObjArray;
namespace edm { class ParameterSet; }

class AnaDataEmul {
public:
  AnaDataEmul(const edm::ParameterSet& cfg);
  void init(TObjArray& histos);
  void run(EventObj* event, L1ObjColl* coll);
  void resume(TObjArray& histos);

private:

  enum DIFF { agree=1, almostAgree=2, ratherAgree = 3, disagree=4, dataOnly=5, emulOnly=6, sizeDiff=7, unknown };
  std::string diffName(const DIFF & diff) const;
  DIFF compare( const L1Obj * data, const L1Obj * emul);

  const L1Obj * bestMatch( const L1Obj * data, const L1ObjColl & emuColl);

  unsigned int hasDtHits(unsigned int hitPattern);
  unsigned int hasCscHits(unsigned int hitPattern);
  unsigned int hasRpcHits(unsigned int hitPattern);
  unsigned int hasRpcHitsB(unsigned int hitPattern);
  unsigned int hasRpcHitsE(unsigned int hitPattern);

  RunEffMap theRunMap;

  
};


#endif

