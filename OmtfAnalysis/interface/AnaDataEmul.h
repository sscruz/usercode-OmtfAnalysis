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

  bool debug;
private:

  int bxMin, bxMax;

  enum MATCH { agree=1, almostAgree=2, ratherAgree = 3, disagree=4, dataOnly=5, emulOnly=6, sizeDiff=7, unknown };
  enum PROBL { ok=0, wrong_size=1,  wrong_board=2, wrong_hits=3,  wrong_kine=4, wrong };
  std::string matchName(const MATCH & ) const;
  std::string problName(const PROBL& ) const;
  MATCH  checkMatch( const L1Obj * data, const L1Obj * emul);
  PROBL  checkProbl( const L1Obj & data, const L1Obj & emul);

  const L1Obj * bestMatch( const L1Obj * data, const L1ObjColl & emuColl);

  unsigned int hasDtHits(unsigned int hitPattern);
  unsigned int hasCscHits(unsigned int hitPattern);
  unsigned int hasRpcHits(unsigned int hitPattern);
  unsigned int hasRpcHitsB(unsigned int hitPattern);
  unsigned int hasRpcHitsE(unsigned int hitPattern);

  RunEffMap theRunMap;

  
};


#endif

