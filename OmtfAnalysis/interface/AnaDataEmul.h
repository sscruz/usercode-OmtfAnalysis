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
  enum DIFF { agree =1, almostAgree=2, notAgree=3, dataOnly=4, emulOnly=5, unknown };
  DIFF compare( const L1Obj * data, const L1Obj * emul);
  bool hasDtHits(unsigned int hitPattern);
  bool hasCscHits(unsigned int hitPattern);
  bool hasRpcHits(unsigned int hitPattern);
};


#endif

