#include "OmtfTreeMaker.h"

#include <vector>

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TFile.h"
#include "TTree.h"

#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"
#include "UserCode/OmtfAnalysis/interface/OmtfGmtData.h"
#include "L1Trigger/L1TMuonOverlap/interface/OmtfName.h"

#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "DataFormats/Math/interface/deltaR.h"

template <class T> T sqr( T t) {return t*t;}

OmtfTreeMaker::OmtfTreeMaker(const edm::ParameterSet& cfg)
  : theConfig(cfg), theCounter(0), theFile(0), theTree(0), 
    bitsL1(0), bitsHLT(0),
    event(0), muon(0), l1ObjColl(0), 
    theMenuInspector(cfg.getParameter<edm::ParameterSet>("menuInspector"), consumesCollector()),
    theBestMuonFinder(cfg.getParameter<edm::ParameterSet>("bestMuonFinder"), consumesCollector()),
    theL1ObjMaker(cfg.getParameter<edm::ParameterSet>("l1ObjMaker"), consumesCollector()) 
{  }
  

void OmtfTreeMaker::beginRun(const edm::Run &ru, const edm::EventSetup &es)
{
  std::cout <<" OmtfTreeMaker::beginRun CALLED" << std::endl; 
  theMenuInspector.checkRun(ru,es);
}

void OmtfTreeMaker::beginJob()
{
  theFile = new TFile(theConfig.getParameter<std::string>("treeFileName").c_str(),"RECREATE");
  theTree = new TTree("tOmtf","OmtfTree");

  theTree->Branch("event","EventObj",&event,32000,99);
  theTree->Branch("muon","MuonObj",&muon,32000,99);
  theTree->Branch("l1ObjColl","L1ObjColl",&l1ObjColl,32000,99);

  theTree->Branch("bitsL1" ,"TriggerMenuResultObj",&bitsL1 ,32000,99);
  theTree->Branch("bitsHLT","TriggerMenuResultObj",&bitsHLT,32000,99);

  theHelper.SetOwner();
  theBestMuonFinder.initHistos(theHelper);
}

void OmtfTreeMaker::endJob()
{
  theFile->Write();
  delete theFile;

  std::string helperFile = theConfig.getParameter<std::string>("histoFileName");
  TFile f(helperFile.c_str(),"RECREATE");
  theHelper.Write();
  f.Close();
}

OmtfTreeMaker::~OmtfTreeMaker()
{
  std::cout <<"OmtfTreeMaker: Event counter is: "<<theCounter<<std::endl;
}

void OmtfTreeMaker::analyze(const edm::Event &ev, const edm::EventSetup &es)
{

  //
  // initial filter. Optionally do not further use events without muon
  //
  const reco::Muon * theMuon = theBestMuonFinder.result(ev,es);
  if (theConfig.getParameter<bool>("onlyBestMuEvents") && (!theMuon) ) return;
  theCounter++;

  //
  // fill event information
  //
  event = new EventObj;
  event->bx = ev.bunchCrossing();
  event->orbit = ev.orbitNumber();
  event->time = ev.time().value();
  event->id = ev.id().event();
  event->run = ev.run();
  event->lumi = ev.luminosityBlock();

  //
  // create other objects structure
  //

  muon = new MuonObj();
  l1ObjColl = new L1ObjColl;


  bitsL1 = new TriggerMenuResultObj();
  bitsHLT = new TriggerMenuResultObj();


  //
  // fill algoBits info
  //
  static edm::RunNumber_t lastRun = 0;
  if (ev.run() != lastRun) {
    lastRun = ev.run();
    bitsL1->names  = theMenuInspector.namesAlgoL1();
    bitsHLT->names = theMenuInspector.namesAlgoHLT();
  }
  bitsL1->firedAlgos = theMenuInspector.firedAlgosL1(ev,es);
  bitsHLT->firedAlgos = theMenuInspector.firedAlgosHLT(ev,es);

  //
  // fill muon info
  //
  muon->isUnique = theBestMuonFinder.isUnique(ev,es);
  muon->nAllMuons = theBestMuonFinder.numberOfAllMuons(ev,es);
  muon->nRPCHits = theBestMuonFinder.numberOfValidMuonRPCHits();
  muon->nDTHits  = theBestMuonFinder.numberOfValidMuonDTHits();
  muon->nCSCHits = theBestMuonFinder.numberOfValidMuonCSCHits();
  muon->nTrackerHits = theBestMuonFinder.numberOfValidTrackerHits();
  if (theMuon) {
    muon->setKine(theMuon->bestTrack()->pt(), theMuon->bestTrack()->eta(), theMuon->bestTrack()->phi(), theMuon->bestTrack()->charge());
    muon->setBits(theMuon->isGlobalMuon(), theMuon->isTrackerMuon(), theMuon->isStandAloneMuon(), theMuon->isCaloMuon(), theMuon->isMatchesValid());
    muon->nMatchedStations = theMuon->numberOfMatchedStations();
  }


  // get L1 candidates
  std::vector<L1Obj> l1Objs = theL1ObjMaker(ev);
  if (l1Objs.size()) {
    l1ObjColl->set(l1Objs);
    l1ObjColl->set( std::vector<bool>(l1Objs.size(),false));
    l1ObjColl->set( std::vector<double>(l1Objs.size(),0.));
  }
    
  
  if (!l1Objs.size() && theMuon) {
    std::cout <<"#"<<theCounter<<" "<< *event << std::endl;
    if (theMuon) std::cout <<"MUON "<<*muon << std::endl;
    for (auto obj : l1Objs) {
     std::cout << obj;
     if (obj.type==L1Obj::OMTF || obj.type==L1Obj::OMTF_emu) std::cout <<" "<<OmtfName(obj.iProcessor,obj.position);
     std::cout << std::endl; 
    }
  }

  //
  // fill ntuple + cleanup
  //
  //if (omtfResult,size()) 
  theTree->Fill();
  delete event; event = 0;
  delete muon;  muon = 0;
  delete bitsL1;  bitsL1= 0;
  delete bitsHLT;  bitsHLT= 0;
  delete l1ObjColl; l1ObjColl = 0;
}
