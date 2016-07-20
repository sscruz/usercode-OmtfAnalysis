#ifndef OmtfTreeMaker_H
#define OmtfTreeMaker_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"

#include "UserCode/OmtfDataFormats/interface/EventObj.h"
#include "UserCode/OmtfDataFormats/interface/MuonObj.h"
#include "UserCode/OmtfDataFormats/interface/MuonObjColl.h"
#include "UserCode/OmtfDataFormats/interface/L1Obj.h"
#include "UserCode/OmtfDataFormats/interface/L1ObjColl.h"
#include "UserCode/OmtfDataFormats/interface/TriggerMenuResultObj.h"

#include "UserCode/OmtfAnalysis/interface/MenuInspector.h"
#include "UserCode/OmtfAnalysis/interface/BestMuonFinder.h"
#include "UserCode/OmtfAnalysis/interface/L1ObjMaker.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"


#include <vector>
#include "TObjArray.h"

namespace edm { class Event; class EventSetup; }
namespace reco { class Muon; }
class TTree;
class TFile;
class TH1F;
class TH2F;

class OmtfTreeMaker : public edm::EDAnalyzer {
public:
  OmtfTreeMaker(const edm::ParameterSet& cfg);
  virtual ~OmtfTreeMaker();
  virtual void beginJob();
  virtual void beginRun(const edm::Run &ru, const edm::EventSetup &es);
  virtual void analyze(const edm::Event &ev, const edm::EventSetup &es);
  virtual void endJob();

  //call back to register consumes. Alternatively possible to declare friend class and call it therein
  template<typename T> edm::EDGetTokenT<T> initConsumes(const edm::InputTag & t) { return consumes<T>(t); }

private:
  edm::ParameterSet theConfig;
  unsigned int theCounter;

  TFile *theFile;
  TTree *theTree;

  TriggerMenuResultObj *bitsL1;
  TriggerMenuResultObj *bitsHLT;

  EventObj* event;
  MuonObj* muon;
  MuonObjColl* muonColl;
  L1ObjColl * l1ObjColl;
								    
  TObjArray      theHelper;

  MenuInspector theMenuInspector;
  BestMuonFinder theBestMuonFinder;
  L1ObjMaker theL1ObjMaker;

};
#endif
