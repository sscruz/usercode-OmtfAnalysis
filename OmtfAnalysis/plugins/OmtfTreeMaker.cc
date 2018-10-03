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

#include "UserCode/OmtfAnalysis/interface/ConverterRPCRawSynchroSynchroCountsObj.h"

template <class T> T sqr( T t) {return t*t;}

OmtfTreeMaker::OmtfTreeMaker(const edm::ParameterSet& cfg)
  : theConfig(cfg), theCounter(0), theFile(0), theTree(0), 
    bitsL1(0), bitsHLT(0),
    event(0), 
    muonColl(0), l1ObjColl(0), 
    synchroCounts(0), closestTrack(0),
    theMenuInspector(cfg.getParameter<edm::ParameterSet>("menuInspector"), consumesCollector()),
    theBestMuonFinder(cfg.getParameter<edm::ParameterSet>("bestMuonFinder"), consumesCollector()),
    theL1ObjMaker(cfg.getParameter<edm::ParameterSet>("l1ObjMaker"), consumesCollector()), 
    theSynchroGrabber(cfg.getParameter<edm::ParameterSet>("linkSynchroGrabber"), consumesCollector()),
    theGenParticleFinder(cfg.getParameter<edm::ParameterSet>("genObjectFinder"), consumesCollector()),
    theClosestTrackFinder(cfg.getParameter<edm::ParameterSet>("closestTrackFinder"), consumesCollector())
{ }
  

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
  theTree->Branch("muonColl", "MuonObjColl", &muonColl, 32000,99);
  theTree->Branch("genColl", "GenObjColl", &muonColl, 32000,99);
  theTree->Branch("l1ObjColl","L1ObjColl",&l1ObjColl,32000,99);

  theTree->Branch("bitsL1" ,"TriggerMenuResultObj",&bitsL1 ,32000,99);
  theTree->Branch("bitsHLT","TriggerMenuResultObj",&bitsHLT,32000,99);
  theTree->Branch("synchroCounts","SynchroCountsObjVect",&synchroCounts,32000,99);
  theTree->Branch("closestTrack","TrackObj",&closestTrack, 32000, 99);

  theHelper.SetOwner();
  theBestMuonFinder.initHistos(theHelper);
  theSynchroGrabber.initHistos(theHelper);
  theClosestTrackFinder.initHistos(theHelper);

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
  muonColl = new MuonObjColl (theBestMuonFinder.muons(ev,es));
  genColl = new GenObjColl;
  l1ObjColl = new L1ObjColl;

  bitsL1 = new TriggerMenuResultObj();
  bitsHLT = new TriggerMenuResultObj();

  synchroCounts = new SynchroCountsObjVect;

  closestTrack = new TrackObj();
  
  // 
  // Get Gen information
  //
  

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
  // associate HLT info to muonColl objs
  //
  theMenuInspector.associateHLT(ev,es,muonColl);


  // get L1 candidates
  std::vector<L1Obj> l1Objs = theL1ObjMaker(ev);
  if (l1Objs.size()) {
    l1ObjColl->set(l1Objs);
    l1ObjColl->set( std::vector<bool>(l1Objs.size(),false));
    l1ObjColl->set( std::vector<double>(l1Objs.size(),0.));
  }
    
  //
  // fill LinkSynchroAnalysis data
  //
  if (theMuon && theMuon->isGlobalMuon()) {
    theSynchroGrabber.setMuon(theMuon);
    RPCRawSynchro::ProdItem rawCounts  = theSynchroGrabber.counts(ev,es);
    synchroCounts->data = ConverterRPCRawSynchroSynchroCountsObj::toSynchroObj(rawCounts);
  }

  
/*  if (l1ObjColl->selectByType(L1Obj::OMTF)) {
  std::cout <<"#"<<theCounter<<" "<< *event << std::endl;
  std::cout << *muonColl << std::endl;
  std::cout << *l1ObjColl << std::endl;
  std::cout << std::endl;
  }
*/

  L1ObjColl omtfColl = l1ObjColl->selectByType(L1Obj::OMTF);
  if (omtfColl) {
    reco::Track track = theClosestTrackFinder.result(ev,es, omtfColl.getL1Objs().front().etaValue(), 
                                                                    omtfColl.getL1Objs().front().phiValue());
    closestTrack->setKine(track.pt(), track.eta(), track.phi(), track.charge());
  }

  //
  // fill ntuple + cleanup
  //
  //if (omtfResult,size()) 
  theTree->Fill();
  delete event; event = 0;
  delete muonColl; muonColl = 0;
  delete genColl; genColl =0;
  delete bitsL1;  bitsL1= 0;
  delete bitsHLT;  bitsHLT= 0;
  delete l1ObjColl; l1ObjColl = 0;
  delete closestTrack; closestTrack = 0;
}
