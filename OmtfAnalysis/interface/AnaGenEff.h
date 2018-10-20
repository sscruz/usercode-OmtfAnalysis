#ifndef UserCode_L1RpcTriggerAnalysis_AnaGenEff_H
#define UserCode_L1RpcTriggerAnalysis_AnaGenEff_H

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TProfile.h"
#include "TObjArray.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TTree.h"
#include "TFile.h"

#include <vector>
#include <map>
#include <string>
#include "UserCode/OmtfAnalysis/interface/Utilities.h"

#include "UserCode/OmtfDataFormats/interface/EventObj.h"
#include "UserCode/OmtfDataFormats/interface/MuonObj.h"
#include "UserCode/OmtfDataFormats/interface/L1Obj.h"
#include "UserCode/OmtfDataFormats/interface/L1ObjColl.h"
#include "UserCode/OmtfAnalysis/interface/Utilities.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "UserCode/OmtfDataFormats/interface/GenObjColl.h"
#include "UserCode/OmtfDataFormats/interface/GenObj.h"



class AnaGenEff { 

 public: 
 AnaGenEff(const edm::ParameterSet & cfg) : debug(false) {}
  void init(TObjArray& histos);
  void run( const EventObj* event, const GenObjColl* muon,
	    const L1ObjColl *l1Coll);
  void resume(TObjArray& histos);
  bool debug;

 private:
  RunEffMap theRunMap;
  std::map<std::string,TH1D* > theHistoMap;
};

#endif
