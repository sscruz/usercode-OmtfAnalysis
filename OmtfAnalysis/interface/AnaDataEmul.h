#ifndef UserCode_OmtfAnalysis_AnaDataEmul_H
#define UserCode_OmtfAnalysis_AnaDataEmul_H

class L1ObjColl;
class L1Obj;
class TObjArray;
namespace edm { class ParameterSet; }

class AnaDataEmul {
public:
  AnaDataEmul(const edm::ParameterSet& cfg);
  void init(TObjArray& histos);
  void run(L1ObjColl* coll);
private:

  enum DIFF { agree=1, almostAgree=2, ratherAgree = 3, notAgree=4, dataOnly=5, emulOnly=6, sizeDiff=7, unknown };
  DIFF compare( const L1Obj * data, const L1Obj * emul);
  const L1Obj * bestMatch( const L1Obj * data, const L1ObjColl & emuColl);

  unsigned int hasDtHits(unsigned int hitPattern);
  unsigned int hasCscHits(unsigned int hitPattern);
  unsigned int hasRpcHits(unsigned int hitPattern);
  unsigned int hasRpcHitsB(unsigned int hitPattern);
  unsigned int hasRpcHitsE(unsigned int hitPattern);

};


#endif

