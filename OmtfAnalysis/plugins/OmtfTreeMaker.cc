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


template <class T> T sqr( T t) {return t*t;}

OmtfTreeMaker::OmtfTreeMaker(const edm::ParameterSet& cfg)
  : theConfig(cfg), theCounter(0), theFile(0), theTree(0), 
    event(0), l1ObjColl(0), 
    theL1ObjMaker(cfg) 
{  
  theOmtfEmulSrc = cfg.getParameter<edm::InputTag>("omtfEmulSrc");
  theOmtfDataSrc = cfg.getParameter<edm::InputTag>("omtfDataSrc");

  theOmtfEmulToken= consumes<l1t::RegionalMuonCandBxCollection>(theOmtfEmulSrc);
  theOmtfDataToken= consumes<l1t::RegionalMuonCandBxCollection>(theOmtfDataSrc);

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
  theTree->Branch("l1ObjColl","L1ObjColl",&l1ObjColl,32000,99);
  theHelper.SetOwner();
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
//  std::cout <<"-----------------------"<< *event << std::endl;
  l1ObjColl = new L1ObjColl;

  // get OMTF candidates
  std::vector<L1Obj> omtfResult;
  getOmtfCandidates(ev, L1Obj::OMTF,  omtfResult);
  getOmtfCandidates(ev, L1Obj::OMTF_emu,  omtfResult);
  if (omtfResult.size()) std::cout << "Size of omtfResult: " << omtfResult.size() << std::endl; 
  if (omtfResult.size()) {
    l1ObjColl->set(omtfResult);
    l1ObjColl->set( std::vector<bool>(omtfResult.size(),false));
    l1ObjColl->set( std::vector<double>(omtfResult.size(),0.));
  }
    
  
  if (omtfResult.size()) {
    std::cout <<"#"<<theCounter<<" "<< *event << std::endl;
    for (auto obj : omtfResult) std::cout << obj << std::endl; 
  }
/*
  for (auto obj : omtfResult) {
    OmtfGmtData::CandMuon c;
    c.pt = obj.pt;
    c.quality = obj.q;
    c.eta = obj.eta;
    c.halo = 0;
    c.phi = obj.phi;
    c.charge = obj.charge; 
    c.valCh = 1;
    c.track = obj.hits;
    std::cout << obj <<" "<< c << std::endl;
  }
*/

  //
  // fill ntuple + cleanup
  //
  //if (omtfResult,size()) 
  theTree->Fill();
  delete event; event = 0;
  delete l1ObjColl; l1ObjColl = 0;
}
bool OmtfTreeMaker::getOmtfCandidates(const edm::Event &iEvent,  L1Obj::TYPE type, std::vector<L1Obj> &result)
{
  int bxNumber = 0;
  edm::Handle<l1t::RegionalMuonCandBxCollection> omtfCandidates; 
  switch (type) {
    case  L1Obj::OMTF_emu: { iEvent.getByToken(theOmtfEmulToken, omtfCandidates); break; }
    case  L1Obj::OMTF    : { iEvent.getByToken(theOmtfDataToken, omtfCandidates); break; }
    default: { std::cout <<"Invalid type : " << type << std::endl; abort(); }
  }
  for (l1t::RegionalMuonCandBxCollection::const_iterator it = omtfCandidates.product()->begin(bxNumber);
       it != omtfCandidates.product()->end(bxNumber);
       ++it) {
    L1Obj obj;
    //obj.eta = it->hwEta()/240.0*2.61;
    obj.eta = it->hwEta();

    ///Add processor offset to phi value
    //float procOffset = (15+it->processor()*60)/180.0*M_PI;
    ///Convert to radians. uGMt has 576 bins for 2Pi  
    //obj.phi = (float)(it->hwPhi())/576.0*2*M_PI+procOffset;    
    //if(obj.phi>M_PI) obj.phi-=2*M_PI;
    obj.phi = it->hwPhi();
    
    
    //obj.pt  = ((float)it->hwPt()-1)/2.0;
    obj.pt = it->hwPt();
    //obj.charge = pow(-1,it->hwSign());
    obj.charge = it->hwSign();

    std::map<int, int> hwAddrMap = it->trackAddress();
    obj.q   = it->hwQual();
    obj.hits   = hwAddrMap[0];
    obj.refLayer   = hwAddrMap[1];
    obj.disc   = hwAddrMap[2];
    //obj.bx  = it->hwSignValid();
    obj.bx = 0;
    obj.type =  type;  //(int)it->trackFinderType();
    obj.iProcessor = it->processor();
    result.push_back(obj);
  }
  return true;
}
				
