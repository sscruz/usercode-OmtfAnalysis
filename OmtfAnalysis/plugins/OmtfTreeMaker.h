#ifndef OmtfTreeMaker_H
#define OmtfTreeMaker_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"

#include "UserCode/OmtfDataFormats/interface/EventObj.h"
#include "UserCode/OmtfDataFormats/interface/MuonObj.h"
#include "UserCode/OmtfDataFormats/interface/L1Obj.h"
#include "UserCode/OmtfDataFormats/interface/L1ObjColl.h"

#include "UserCode/OmtfAnalysis/interface/L1ObjMaker.h"
#include "UserCode/OmtfAnalysis/interface/BestMuonFinder.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"


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

private:
  bool  getOmtfCandidates(const edm::Event &iEvent, 
                          L1Obj::TYPE type,
                          std::vector<L1Obj> &result);


private:
  edm::ParameterSet theConfig;
  unsigned int theCounter;

  TFile *theFile;
  TTree *theTree;

  EventObj* event;
  MuonObj* muon;
  L1ObjColl * l1ObjColl;
  
								    
  TObjArray      theHelper;

  BestMuonFinder theBestMuonFinder;
  L1ObjMaker theL1ObjMaker;

private:
  edm::InputTag theOmtfEmulSrc, theOmtfDataSrc;
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> theOmtfEmulToken;
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> theOmtfDataToken;

  edm::EDGetTokenT<reco::MuonCollection> theBestMuon_Tag;

};
#endif
