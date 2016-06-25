//#include "OmtfTreeMaker.h"
#include "UserCode/OmtfAnalysis/interface/OmtfTreeMaker.h"

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
    event(0), muon(0), l1ObjColl(0), 
    theBestMuonFinder(cfg.getParameter<edm::ParameterSet>("bestMuonFinder")),
    theL1ObjMaker(cfg) 
{  
  theOmtfEmulSrc = cfg.getParameter<edm::InputTag>("omtfEmulSrc");
  theOmtfDataSrc = cfg.getParameter<edm::InputTag>("omtfDataSrc");
  theBmtfDataSrc = cfg.getParameter<edm::InputTag>("bmtfDataSrc");
  theEmtfDataSrc = cfg.getParameter<edm::InputTag>("emtfDataSrc");

  theOmtfEmulToken= consumes<l1t::RegionalMuonCandBxCollection>(theOmtfEmulSrc);
  theOmtfDataToken= consumes<l1t::RegionalMuonCandBxCollection>(theOmtfDataSrc);
  theBmtfDataToken= consumes<l1t::RegionalMuonCandBxCollection>(theBmtfDataSrc);
  theEmtfDataToken= consumes<l1t::RegionalMuonCandBxCollection>(theEmtfDataSrc);

/*
  theBestMuon_Tag = consumes<reco::MuonCollection> (theConfig
                             .getParameter<edm::ParameterSet>("bestMuonFinder")
                             .getParameter<std::string>("muonColl") );
*/

  //theBeamSpotToken =  consumes<reco::BeamSpot>(edm::InputTag("offlineBeamSpot"));

  theBestMuonFinder.initConsumes(this);

}
  

void OmtfTreeMaker::beginRun(const edm::Run &ru, const edm::EventSetup &es)
{
  std::cout <<" OmtfTreeMaker::beginRun CALLED" << std::endl; 
}

void OmtfTreeMaker::beginJob()
{
  theFile = new TFile(theConfig.getParameter<std::string>("treeFileName").c_str(),"RECREATE");
  theTree = new TTree("tOmtf","OmtfTree");

  theTree->Branch("event","EventObj",&event,32000,99);
  theTree->Branch("muon","MuonObj",&muon,32000,99);
  theTree->Branch("l1ObjColl","L1ObjColl",&l1ObjColl,32000,99);

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
  // initial filter. Do no use events without muon
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
  std::vector<L1Obj> l1Objs;
  getOmtfCandidates(ev, L1Obj::OMTF,  l1Objs);
  getOmtfCandidates(ev, L1Obj::OMTF_emu,  l1Objs);
  getOmtfCandidates(ev, L1Obj::EMTF,  l1Objs);
  getOmtfCandidates(ev, L1Obj::BMTF,  l1Objs);
//  if (l1Objs.size()) std::cout << "Size of l1Objs: " << l1Objs.size() << std::endl; 
  if (l1Objs.size()) {
    l1ObjColl->set(l1Objs);
    l1ObjColl->set( std::vector<bool>(l1Objs.size(),false));
    l1ObjColl->set( std::vector<double>(l1Objs.size(),0.));
  }
    
  
  if (l1Objs.size() || theMuon) {
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
  delete l1ObjColl; l1ObjColl = 0;
}


// FIXME GET RID OF THIS FROM HERE!
bool OmtfTreeMaker::getOmtfCandidates(const edm::Event &iEvent,  L1Obj::TYPE type, std::vector<L1Obj> &result)
{
  int bxNumber = 0;
  edm::Handle<l1t::RegionalMuonCandBxCollection> candidates; 
  switch (type) {
    case  L1Obj::OMTF_emu: { iEvent.getByToken(theOmtfEmulToken, candidates); break; }
    case  L1Obj::OMTF    : { iEvent.getByToken(theOmtfDataToken, candidates); break; }
    case  L1Obj::BMTF    : { iEvent.getByToken(theBmtfDataToken, candidates); break; }
    case  L1Obj::EMTF    : { iEvent.getByToken(theEmtfDataToken, candidates); break; }
    default: { std::cout <<"Invalid type : " << type << std::endl; abort(); }
  }
  for (l1t::RegionalMuonCandBxCollection::const_iterator it = candidates.product()->begin(bxNumber);
       it != candidates.product()->end(bxNumber);
       ++it) {

    L1Obj obj;
    obj.type =  type;  
    obj.iProcessor = it->processor(); // [0..5]

    obj.position =   (it->trackFinderType() == l1t::omtf_neg || it->trackFinderType() == l1t::emtf_neg) ? -1 : 
                   ( (it->trackFinderType() == l1t::omtf_pos || it->trackFinderType() == l1t::emtf_pos) ? +1 : 0 ); 

    obj.phi = it->hwPhi();  // phi_Rad = [ (15.+processor*60.)/360. + hwPhi/576. ] *2*M_PI 
    obj.eta = it->hwEta();  // eta = hwEta/240.*2.61
    obj.pt = it->hwPt();         // pt = (hwPt-1.)/2.
    obj.charge = it->hwSign();   // charge=  pow(-1,hwSign)

    std::map<int, int> hwAddrMap = it->trackAddress();
    obj.q   = it->hwQual();
    obj.hits   = hwAddrMap[0];
    obj.bx = bxNumber;
    result.push_back(obj);
  }
  return true;
}
				
