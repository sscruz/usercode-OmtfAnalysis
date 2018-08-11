#include "UserCode/OmtfAnalysis/interface/L1ObjMaker.h"

#include <iostream>
#include <sstream>

#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"

#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTExtendedCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "L1Trigger/RPCTrigger/interface/RPCConst.h"

#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"
#include "DataFormats/L1Trigger/interface/Muon.h"


using namespace std;
namespace {
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> theOmtfEmulToken, theOmtfDataToken, theEmtfDataToken, theBmtfDataToken;
  edm::EDGetTokenT<l1t::MuonBxCollection> theGmtDataToken, theGmtEmulToken;
}

L1ObjMaker::L1ObjMaker(const  edm::ParameterSet & cfg, edm::ConsumesCollector&& cColl)
  :  theConfig(cfg),
     lastEvent(0),lastRun(0)
{
  if (theConfig.exists("omtfEmulSrc")) theOmtfEmulToken =  cColl.consumes<l1t::RegionalMuonCandBxCollection>(  theConfig.getParameter<edm::InputTag>("omtfEmulSrc") );
  if (theConfig.exists("omtfDataSrc")) theOmtfDataToken =  cColl.consumes<l1t::RegionalMuonCandBxCollection>(  theConfig.getParameter<edm::InputTag>("omtfDataSrc") );
  if (theConfig.exists("bmtfDataSrc")) theBmtfDataToken =  cColl.consumes<l1t::RegionalMuonCandBxCollection>(  theConfig.getParameter<edm::InputTag>("bmtfDataSrc") );
  if (theConfig.exists("emtfDataSrc")) theEmtfDataToken =  cColl.consumes<l1t::RegionalMuonCandBxCollection>(  theConfig.getParameter<edm::InputTag>("emtfDataSrc") );
  if (theConfig.exists("gmtDataSrc"))  theGmtDataToken  =  cColl.consumes<l1t::MuonBxCollection>( theConfig.getParameter<edm::InputTag>("gmtDataSrc") );
  if (theConfig.exists("gmtEmulSrc"))  theGmtEmulToken  =  cColl.consumes<l1t::MuonBxCollection>( theConfig.getParameter<edm::InputTag>("gmtEmulSrc") );
 
}

void L1ObjMaker::run(const edm::Event &ev)
{
  if (lastEvent==ev.id().event() && lastRun==ev.run()) return;
  lastEvent = ev.id().event() ;
  lastRun = ev.run();
  theL1Objs.clear();

  if ( !theGmtDataToken.isUninitialized())  makeGmtCandidates(ev, L1Obj::uGMT    , theL1Objs);
  if ( !theGmtEmulToken.isUninitialized())  makeGmtCandidates(ev, L1Obj::uGMT_emu, theL1Objs);
  if (!theOmtfDataToken.isUninitialized())  makeRegCandidates(ev, L1Obj::OMTF    , theL1Objs);
  if (!theOmtfEmulToken.isUninitialized())  makeRegCandidates(ev, L1Obj::OMTF_emu, theL1Objs);
  if (!theBmtfDataToken.isUninitialized())  makeRegCandidates(ev, L1Obj::BMTF    , theL1Objs);
  if (!theEmtfDataToken.isUninitialized())  makeRegCandidates(ev, L1Obj::EMTF    , theL1Objs);

/*
  if (theConfig.exists("l1RpcSource"))     getGMTReadout( ev, theL1Objs, theConfig.getParameter<edm::InputTag>("l1RpcSource"),    L1Obj::RPCb);
  if (theConfig.exists("l1RpcSource"))     getGMTReadout( ev, theL1Objs, theConfig.getParameter<edm::InputTag>("l1RpcSource"),    L1Obj::RPCf);
  if (theConfig.exists("l1CscSource"))     getGMTReadout( ev, theL1Objs, theConfig.getParameter<edm::InputTag>("l1CscSource"),    L1Obj::CSC);
  if (theConfig.exists("l1DtSource") )     getGMTReadout( ev, theL1Objs, theConfig.getParameter<edm::InputTag>("l1DtSource"),     L1Obj::DT);
  if (theConfig.exists("l1GmtSource"))     getGMTReadout( ev, theL1Objs, theConfig.getParameter<edm::InputTag>("l1GmtSource"),    L1Obj::GMT);
  if (theConfig.exists("l1RpcEmuSource"))  getRpcRegional(ev, theL1Objs, theConfig.getParameter<edm::InputTag>("l1RpcEmuSource"));
  if (theConfig.exists("l1GmtEmuSource"))  getGMTReadout( ev, theL1Objs, theConfig.getParameter<edm::InputTag>("l1GmtEmuSource"), L1Obj::GMT_emu);
*/

}

bool L1ObjMaker::makeGmtCandidates(const edm::Event &iEvent,  L1Obj::TYPE type, std::vector<L1Obj> &result)
{
  edm::Handle<l1t::MuonBxCollection> candidates;
  switch (type) {
    case  L1Obj::uGMT     : { iEvent.getByToken(theGmtDataToken, candidates); break; }
    case  L1Obj::uGMT_emu : { iEvent.getByToken(theGmtEmulToken, candidates); break; }
    default: { std::cout <<"Invalid type : " << type << std::endl; abort(); }
  }
//  int bxNumber = 0;
  for (int bxNumber=-2; bxNumber<=2; bxNumber++) {
  for (l1t::MuonBxCollection::const_iterator it = candidates.product()->begin(bxNumber);
      it != candidates.product()->end(bxNumber);
      ++it) {
    L1Obj obj;
    obj.type =  type;
    obj.phi = it->hwPhi(); 
    obj.eta = it->hwEta();        // eta = hwEta/240.*2.61
    obj.pt  = it->hwPt();         // pt = (hwPt-1.)/2.
    obj.q   = it->hwQual();                             
    obj.bx = bxNumber;
    obj.charge = it->hwCharge();  // charge  =  pow(-1,hwSign)
    result.push_back(obj);
  }
  }
  return true; 
}

bool L1ObjMaker::makeRegCandidates(const edm::Event &iEvent,  L1Obj::TYPE type, std::vector<L1Obj> &result)
{
  edm::Handle<l1t::RegionalMuonCandBxCollection> candidates;
  switch (type) {
    case  L1Obj::OMTF_emu: { iEvent.getByToken(theOmtfEmulToken, candidates); break; }
    case  L1Obj::OMTF    : { iEvent.getByToken(theOmtfDataToken, candidates); break; }
    case  L1Obj::BMTF    : { iEvent.getByToken(theBmtfDataToken, candidates); break; }
    case  L1Obj::EMTF    : { iEvent.getByToken(theEmtfDataToken, candidates); break; }
    default: { std::cout <<"Invalid type : " << type << std::endl; abort(); }
  }
  //int bxNumber = 0;
  for (int bxNumber=-2; bxNumber<=2; bxNumber++) {
  if (bxNumber!=0 && type==L1Obj::OMTF_emu) continue;
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
    obj.disc = hwAddrMap[2];
    result.push_back(obj);
  }
  }
  return true;
}


/*
bool L1ObjMaker::getGMTReadout(const edm::Event &ev, vector<L1Obj> &result, const edm::InputTag &readout, L1Obj::TYPE type)
{
  edm::Handle<L1MuGMTReadoutCollection> pCollection;
  ev.getByLabel(readout,pCollection);
  if (!pCollection.isValid()) return false;

  L1MuGMTReadoutCollection const* gmtrc = pCollection.product();
  if (!gmtrc) return false;

  vector<L1MuGMTReadoutRecord> gmt_records = gmtrc->getRecords();
  vector<L1MuGMTReadoutRecord>::const_iterator RRItr;
  for( RRItr = gmt_records.begin() ; RRItr != gmt_records.end() ; RRItr++ ) {
    vector<L1MuRegionalCand>    cands;
    vector<L1MuGMTExtendedCand> gmts;
    switch (type) {
      case L1Obj::RPCb: { cands = RRItr->getBrlRPCCands(); break; }
      case L1Obj::RPCf: { cands = RRItr->getFwdRPCCands(); break; }
      case L1Obj::DT:   { cands = RRItr->getDTBXCands(); break; }
      case L1Obj::CSC:  { cands = RRItr->getCSCCands(); break; }
      case L1Obj::GMT:      { gmts  = RRItr->getGMTCands(); break; }
      case L1Obj::GMT_emu:  { gmts  = RRItr->getGMTCands(); break; }
      default: break;
    };

    typedef vector<L1MuRegionalCand>::const_iterator ITC;
    for(ITC it = cands.begin() ; it != cands.end() ; ++it ) {
      if (it->empty()) continue;
      result.push_back( makeL1Obj( *it, type) );
    }

    typedef vector<L1MuGMTExtendedCand>::const_iterator ITG;
    for(ITG it = gmts.begin() ; it != gmts.end() ; ++it ) {
      if (it->empty()) continue;
      result.push_back( makeL1Obj( *it, type) );
    }

  }
  return true;
}
*/
