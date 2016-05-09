#include "UserCode/OmtfAnalysis/interface/L1ObjMaker.h"

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

#include <iostream>
#include <fstream>

using namespace std;

L1ObjMaker::L1ObjMaker(const  edm::ParameterSet & cfg)
  :  theConfig(cfg),
     lastEvent(0),lastRun(0)
{}

void L1ObjMaker::run(const edm::Event &ev)
{
  if (lastEvent==ev.id().event() && lastRun==ev.run()) return;
  lastEvent = ev.id().event() ;
  lastRun = ev.run();
  theL1Objs.clear();

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
bool L1ObjMaker::getOmtfCandidates(const edm::Event &ev, std::vector<L1Obj> &result, const edm::InputTag &readout, L1Obj::TYPE t)
{
/*
  int bxNumber = 0;
  edm::Handle<l1t::RegionalMuonCandBxCollection> omtfCandidates; 
  iEvent.getByToken(inputOMTFToken, omtfCandidates);
  for (l1t::RegionalMuonCandBxCollection::const_iterator it = omtfCandidates.product()->begin(bxNumber);
       it != omtfCandidates.product()->end(bxNumber);
       ++it) {
    L1Obj obj;
    obj.eta = it->hwEta()/240.0*2.61;
    ///Add processor offset to phi value
    float procOffset = (15+it->processor()*60)/180.0*M_PI;
    ///Convert to radians. uGMt has 576 bins for 2Pi  
    obj.phi = (float)(it->hwPhi())/576.0*2*M_PI+procOffset;    
    if(obj.phi>M_PI) obj.phi-=2*M_PI;
    
    obj.pt  = ((float)it->hwPt()-1)/2.0;
    obj.charge = pow(-1,it->hwSign());

    std::map<int, int> hwAddrMap = it->trackAddress();
    obj.q   = it->hwQual();
    obj.hits   = hwAddrMap[0];
    obj.refLayer   = hwAddrMap[1];
    obj.disc   = hwAddrMap[2];
    obj.bx  = it->hwSignValid();
    obj.type =  L1Obj::OMTF;//(int)it->trackFinderType();
    obj.iProcessor = it->processor();
    result.push_back(obj);
  }
*/
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


