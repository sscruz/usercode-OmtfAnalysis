// system include files
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <map>
#include <string>

// user include files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"

#include "CondFormats/RPCObjects/interface/RPCReadOutMapping.h"
#include "CondFormats/RPCObjects/interface/LinkBoardSpec.h"
#include "FWCore/Framework/interface/ESTransientHandle.h"
#include "CondFormats/RPCObjects/interface/RPCEMap.h"
#include "CondFormats/DataRecord/interface/RPCEMapRcd.h"
#include "CondFormats/RPCObjects/interface/DccSpec.h"
#include "CondFormats/RPCObjects/interface/LinkBoardElectronicIndex.h"
#include "CondFormats/RPCObjects/interface/LinkBoardPackedStrip.h"
#include "CondFormats/RPCObjects/interface/TriggerBoardSpec.h"
#include "CondFormats/RPCObjects/interface/LinkBoardSpec.h"

#include "EventFilter/RPCRawToDigi/interface/RPCRecordFormatter.h"
#include "DataFormats/RPCDigi/interface/DataRecord.h"
#include "DataFormats/RPCDigi/interface/ReadoutError.h"
#include "DataFormats/RPCDigi/interface/RPCRawSynchro.h"
#include "EventFilter/RPCRawToDigi/interface/EventRecords.h"
#include "EventFilter/RPCRawToDigi/interface/DebugDigisPrintout.h"

#include "DataFormats/RPCDigi/interface/DataRecord.h"
#include "DataFormats/RPCDigi/interface/RecordBX.h"
#include "DataFormats/RPCDigi/interface/RecordSLD.h"
#include "DataFormats/RPCDigi/interface/RecordCD.h"
#include "DataFormats/RPCDigi/interface/RPCDigi.h"
#include "EventFilter/RPCRawToDigi/interface/DebugDigisPrintout.h"

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

    struct MyDigi { 
       uint32_t det; int strip; int bx; 
       bool operator==(const MyDigi&o) const {
         return (det==o.det && strip==o.strip && bx==o.bx);
       } 
       bool operator< (const MyDigi&o) const { 
         if (this->det < o.det) return true;
         if (this->det > o.det) return false; 
         if (this->strip < o.strip) return true;
         if (this->strip > o.strip) return false;
         if (this->bx < o.bx) return true;
         return false;
       }
    };



class OmtfDigiCompare : public edm::EDAnalyzer {
public:
  OmtfDigiCompare (const edm::ParameterSet & cfg);
  virtual ~OmtfDigiCompare(){}

  virtual void analyze(const edm::Event&, const edm::EventSetup& es);

private:
    edm::EDGetTokenT<RPCDigiCollection> inputRPC_PACT, inputRPC_OMTF;

    unsigned int omtfDigis, omtfDigisError; 
};


OmtfDigiCompare::OmtfDigiCompare(const edm::ParameterSet & cfg) : omtfDigis(0), omtfDigisError(0) {
  inputRPC_PACT = consumes<RPCDigiCollection>(cfg.getParameter<edm::InputTag>("srcRPC_PACT"));
  inputRPC_OMTF = consumes<RPCDigiCollection>(cfg.getParameter<edm::InputTag>("srcRPC_OMTF"));
}

void OmtfDigiCompare::analyze(const edm::Event &ev, const edm::EventSetup& es)
{
  std::cout << "-------- HERE OMTF DIGI COMPARE ---------" << std::endl;
  edm::Handle< RPCDigiCollection > digiCollectionRPC_PACT;
  ev.getByToken(inputRPC_PACT,digiCollectionRPC_PACT);

  edm::Handle< RPCDigiCollection > digiCollectionRPC_OMTF;
  ev.getByToken(inputRPC_OMTF,digiCollectionRPC_OMTF);


  typedef  DigiContainerIterator<RPCDetId, RPCDigi> DigiRangeIterator;

  std::vector<MyDigi> myPact;
  for (DigiRangeIterator it=digiCollectionRPC_PACT->begin(); it != digiCollectionRPC_PACT->end(); it++) {
    RPCDetId rpcDetId = (*it).first;
    uint32_t rawDetId = rpcDetId.rawId();
    RPCDigiCollection::Range range = digiCollectionRPC_PACT->get(rpcDetId);
    for (std::vector<RPCDigi>::const_iterator  id = range.first; id != range.second; id++) {
      const RPCDigi & digi = (*id);
      MyDigi myDigi = { rawDetId, digi.strip(), digi.bx() };
      if (myPact.end() == std::find(myPact.begin(), myPact.end(), myDigi)) myPact.push_back(myDigi);
    } 
  }
  std::sort(myPact.begin(),myPact.end());
  std::cout <<" myPact size is: " << myPact.size() << std::endl;

  std::vector<MyDigi> myOmtf;
  for (DigiRangeIterator it=digiCollectionRPC_OMTF->begin(); it != digiCollectionRPC_OMTF->end(); it++) {
    RPCDetId rpcDetId = (*it).first;
    uint32_t rawDetId = rpcDetId.rawId();
    RPCDigiCollection::Range range = digiCollectionRPC_OMTF->get(rpcDetId);
    for (std::vector<RPCDigi>::const_iterator  id = range.first; id != range.second; id++) {
      const RPCDigi & digi = (*id);
      MyDigi myDigi = { rawDetId, digi.strip(), digi.bx() };
      if (myOmtf.end() == std::find(myOmtf.begin(), myOmtf.end(), myDigi)) myOmtf.push_back(myDigi);
    } 
  }
  std::sort(myOmtf.begin(),myOmtf.end());
  std::cout <<" myOmtf size is: " << myOmtf.size() << std::endl;

  bool hasError = false;
  for (const auto & omtf : myOmtf ) {
     if (omtf.bx >= 4) continue;
     if (omtf.bx != 0) continue;
     omtfDigis++;     
     std::vector<MyDigi>::const_iterator itRpc = find(myPact.begin(), myPact.end(), omtf);
     if (itRpc == myPact.end() ) {
       std::cout << "HERE PROBLEM!!! ----- RPC DIGI corresponding to OMTF (det:"<<omtf.det<<") NOT FOUND! " << std::endl;
       hasError = true;
       omtfDigisError++; 
     }
  }

  if (hasError) {
    std::cout <<"PACT: " << rpcrawtodigi::DebugDigisPrintout()(digiCollectionRPC_PACT.product()) << std::endl;
    std::cout <<"OMTF: " <<  rpcrawtodigi::DebugDigisPrintout()(digiCollectionRPC_OMTF.product()) << std::endl;
  }

  std::cout <<" All OMTF: " << omtfDigis <<", with Error: "<< omtfDigisError << std::endl;
  
}

DEFINE_FWK_MODULE(OmtfDigiCompare);
