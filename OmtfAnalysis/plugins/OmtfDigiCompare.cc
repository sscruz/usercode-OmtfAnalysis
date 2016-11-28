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

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"

//#include "DataFormats/MuonDetId/interface/DtDetId.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"


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
       friend std::ostream & operator<< (std::ostream &out, const MyDigi &o) {
         out<<"MyDigi det: "<<o.det<<", strip: "<<o.strip<<", bx: "<<o.bx; 
         return out;
       }
    };



class OmtfDigiCompare : public edm::EDAnalyzer {
public:
  OmtfDigiCompare (const edm::ParameterSet & cfg);
  virtual ~OmtfDigiCompare(){}

  virtual void analyze(const edm::Event &ev, const edm::EventSetup& es) {
    analyzeDT(ev,es);
//    analyzeCSC(ev,es);
//    analyzeRPC(ev,es);
  }
  void analyzeDT(const edm::Event&, const edm::EventSetup& es);
  void analyzeCSC(const edm::Event&, const edm::EventSetup& es);
  void analyzeRPC(const edm::Event&, const edm::EventSetup& es);

private:
    edm::EDGetTokenT<RPCDigiCollection> inputRPC_PACT, inputRPC_OMTF;
    edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection> inputCSC_CSC, inputCSC_OMTF;
    edm::EDGetTokenT<L1MuDTChambPhContainer> inputDTPh_BMTF;
    edm::EDGetTokenT<L1MuDTChambThContainer> inputDTTh_BMTF;

    unsigned int omtfDigis, omtfDigisError; 
};


OmtfDigiCompare::OmtfDigiCompare(const edm::ParameterSet & cfg) : omtfDigis(0), omtfDigisError(0) {
  inputRPC_PACT = consumes<RPCDigiCollection>(cfg.getParameter<edm::InputTag>("srcRPC_PACT"));
  inputRPC_OMTF = consumes<RPCDigiCollection>(cfg.getParameter<edm::InputTag>("srcRPC_OMTF"));
  inputCSC_CSC = consumes<CSCCorrelatedLCTDigiCollection>(cfg.getParameter<edm::InputTag>("srcCSC_CSC"));
  inputCSC_OMTF = consumes<CSCCorrelatedLCTDigiCollection>(cfg.getParameter<edm::InputTag>("srcCSC_OMTF"));
  inputDTPh_BMTF = consumes<L1MuDTChambPhContainer>(cfg.getParameter<edm::InputTag>("srcDTPh_BMTF"));
  inputDTTh_BMTF = consumes<L1MuDTChambThContainer>(cfg.getParameter<edm::InputTag>("srcDTTh_BMTF"));
}

void OmtfDigiCompare::analyzeRPC(const edm::Event &ev, const edm::EventSetup& es)
{
  std::cout << "-------- HERE DIGI COMPARE RPC---------" << std::endl;
  edm::Handle< RPCDigiCollection > digiCollectionRPC_PACT;
  ev.getByToken(inputRPC_PACT,digiCollectionRPC_PACT);

  edm::Handle< RPCDigiCollection > digiCollectionRPC_OMTF;
  ev.getByToken(inputRPC_OMTF,digiCollectionRPC_OMTF);


  typedef  DigiContainerIterator<RPCDetId, RPCDigi> DigiRangeIterator;


  std::vector<MyDigi> myOmtf;
  std::cout <<" RPC digis from OMTF"<<std::endl;
  for (DigiRangeIterator it=digiCollectionRPC_OMTF->begin(); it != digiCollectionRPC_OMTF->end(); it++) {
    RPCDetId rpcDetId = (*it).first;
    uint32_t rawDetId = rpcDetId.rawId();
    RPCDigiCollection::Range range = digiCollectionRPC_OMTF->get(rpcDetId);
    for (std::vector<RPCDigi>::const_iterator  id = range.first; id != range.second; id++) {
      const RPCDigi & digi = (*id);
      MyDigi myDigi = { rawDetId, digi.strip(), digi.bx() };
      if (myOmtf.end() == std::find(myOmtf.begin(), myOmtf.end(), myDigi)) myOmtf.push_back(myDigi);
//      std::cout <<rpcDetId<< " myDigi:"<<myDigi<<std::endl;
    } 
  }
  std::sort(myOmtf.begin(),myOmtf.end());
  std::cout <<" myOmtf size is: " << myOmtf.size() << std::endl;

  std::vector<MyDigi> myPact;
  std::cout <<" RPC digis from PACT"<<std::endl;
  for (DigiRangeIterator it=digiCollectionRPC_PACT->begin(); it != digiCollectionRPC_PACT->end(); it++) {
    RPCDetId rpcDetId = (*it).first;
    if (rpcDetId.region()==0 && abs(rpcDetId.ring())!=2 ) continue;
    if (rpcDetId.region()==0 && abs(rpcDetId.ring())==2 && rpcDetId.station()==4 && rpcDetId.roll()==1 ) continue;
    if (rpcDetId.region()==0 && abs(rpcDetId.ring())==2 && rpcDetId.station()==3 && rpcDetId.roll()==1 ) continue;
    if (abs(rpcDetId.region())==1 && rpcDetId.station()==4) continue;
    if (abs(rpcDetId.region())==1 && rpcDetId.station()!=1 && rpcDetId.ring()!=3) continue;
    if (abs(rpcDetId.region())==1 && rpcDetId.station()==1 && rpcDetId.ring()<2) continue;
    uint32_t rawDetId = rpcDetId.rawId();
    RPCDigiCollection::Range range = digiCollectionRPC_PACT->get(rpcDetId);
    for (std::vector<RPCDigi>::const_iterator  id = range.first; id != range.second; id++) {
      const RPCDigi & digi = (*id);
      MyDigi myDigi = { rawDetId, digi.strip(), digi.bx() };
      if (myPact.end() == std::find(myPact.begin(), myPact.end(), myDigi)) myPact.push_back(myDigi);
//      std::cout <<rpcDetId<< " myDigi:"<<myDigi<<std::endl;
      
    } 
  }
  std::sort(myPact.begin(),myPact.end());
  std::cout <<" myPact size is: " << myPact.size() << std::endl;

  bool hasError = false;
  for (const auto & omtf : myOmtf ) {
     if (omtf.bx != 0) continue;
     omtfDigis++;     
     std::vector<MyDigi>::const_iterator itRpc = find(myPact.begin(), myPact.end(), omtf);
     if (itRpc == myPact.end() ) {
       std::cout << "HERE PROBLEM!!! ----- RPC DIGI corresponding to OMTF ("<<RPCDetId(omtf.det)<<" digi:"<<omtf<<") NOT FOUND! " << std::endl;
       hasError = true;
       omtfDigisError++; 
     }
  }
  for (const auto & pact : myPact) {
     if (pact.bx != 0) continue;
     omtfDigis++;     
     std::vector<MyDigi>::const_iterator it = find(myOmtf.begin(), myOmtf.end(), pact);
     if (it == myOmtf.end() ) {
       std::cout << "HERE PROBLEM!!! ----- OMTF DIGI corresponding to PACT ("<<RPCDetId(pact.det)<<" digi:"<<pact<<") NOT FOUND! " << std::endl;
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
void OmtfDigiCompare::analyzeDT( const edm::Event &ev, const edm::EventSetup& es) {
  std::cout << "-------- HERE DIGI COMPARE DT ---------" << std::endl;
  edm::Handle<L1MuDTChambPhContainer> digiCollectionDTPh_BMTF;
  ev.getByToken(inputDTPh_BMTF, digiCollectionDTPh_BMTF);
  const L1MuDTChambPhContainer& dtphDigisBMTF = *digiCollectionDTPh_BMTF.product();
  std::cout <<" DTPh digis from BMTF " << dtphDigisBMTF.getContainer()->size()<< std::endl;
  for (const auto &  chDigi : *dtphDigisBMTF.getContainer() ) {
    std::cout <<"HERE DtDataWord64 BMTF " 
        <<" bxNum: "<<chDigi.bxNum()
        <<" whNum: "<<chDigi.whNum()
        <<" station: "<< chDigi.stNum()
        <<" sector: "<< chDigi.scNum()
        <<" code(q): "<< chDigi.code()
        <<" phi:   "<<chDigi.phi()
        << std::endl;

  }
  edm::Handle<L1MuDTChambThContainer> digiCollectionDTTh_BMTF;
  ev.getByToken(inputDTTh_BMTF, digiCollectionDTTh_BMTF);
  const L1MuDTChambThContainer& dtthDigisBMTF = *digiCollectionDTTh_BMTF.product();
  for (const auto &  chDigi : *dtthDigisBMTF.getContainer() ) {

/*
    unsigned int eta = 0;
    unsigned int etaQ = 0;
    for (unsigned int ipos=0; ipos <7; ipos++) {
     if (
    }
*/
    
    std::cout <<"HERE DtDataWord64 BMTF TH " 
        <<" bxNum: "<<chDigi.bxNum()
        <<" whNum: "<<chDigi.whNum()
        <<" station: "<< chDigi.stNum()
        <<" sector: "<< chDigi.scNum()
        << std::endl;

  }

  
}

void OmtfDigiCompare::analyzeCSC(const edm::Event &ev, const edm::EventSetup& es) {
  std::cout << "-------- HERE DIGI COMPARE CSC---------" << std::endl;

  edm::Handle<CSCCorrelatedLCTDigiCollection> digiCollectionCSC_OMTF;
  ev.getByToken(inputCSC_OMTF,digiCollectionCSC_OMTF);
  const CSCCorrelatedLCTDigiCollection & cscDigisOmtf = *digiCollectionCSC_OMTF.product();
  std::cout <<" CSC digis from OMTF" << std::endl;
  std::vector<MyDigi> myOmtf;
  for (const auto & chDigis : cscDigisOmtf) {
    auto rawId = chDigis.first;
    CSCDetId cscDetId(rawId);
    std::cout <<"--------------"<< std::endl;
    std::cout <<"CSC DET ID: "<< cscDetId << std::endl; 
    for (auto digi = chDigis.second.first; digi != chDigis.second.second; digi++) {
      std::cout << *digi << std::endl;
      MyDigi myDigi = { rawId, digi->getStrip(), digi->getBX()-3 };
      std::cout <<" MyDigi (OMTF): " << myDigi << std::endl;
      if (myOmtf.end() == std::find(myOmtf.begin(), myOmtf.end(), myDigi)) myOmtf.push_back(myDigi);
      else std::cout <<" DUPLICATE: " << std::endl;
    }
  } 
  std::sort(myOmtf.begin(),myOmtf.end());
  std::cout <<" myOmtf size is: " << myOmtf.size() << std::endl;
  
  std::vector<MyDigi> myCsc;
  edm::Handle<CSCCorrelatedLCTDigiCollection> digiCollectionCSC_CSC;
  ev.getByToken(inputCSC_CSC,digiCollectionCSC_CSC);
  const CSCCorrelatedLCTDigiCollection & cscDigisCSC = *digiCollectionCSC_CSC.product();
  std::cout <<" CSC digis from CSC" << std::endl;
  for (const auto & chDigis : cscDigisCSC) {
    auto rawId = chDigis.first;
    CSCDetId cscDetId(rawId);
    if (cscDetId.ring()==1)continue;
    std::cout <<"--------------"<< std::endl;
    std::cout <<"CSC DET ID: "<< cscDetId << std::endl; 
    for (auto digi = chDigis.second.first; digi != chDigis.second.second; digi++) {
      std::cout << *digi << std::endl;
      MyDigi myDigi = { rawId, digi->getStrip(), digi->getBX() };
      if (myCsc.end() == std::find(myCsc.begin(), myCsc.end(), myDigi)) myCsc.push_back(myDigi);
    }
  } 
  std::sort(myCsc.begin(),myCsc.end());
  std::cout <<" myCsc size is: " << myCsc.size() << std::endl;

  bool hasError = false;
  for (const auto & omtf : myOmtf ) {
     if (omtf.bx != 3) continue;
     omtfDigis++;     
     std::vector<MyDigi>::const_iterator itCsc = find(myCsc.begin(), myCsc.end(), omtf);
     if (itCsc == myCsc.end() ) {
       std::cout << "HERE PROBLEM!!! ----- CSC DIGI corresponding to OMTF ("<<omtf<<") NOT FOUND! " << std::endl;
       hasError = true;
       omtfDigisError++; 
     }
  }
  for (const auto & csctf : myCsc ) {
     if (csctf.bx != 3) continue;
     omtfDigis++;     
     std::vector<MyDigi>::const_iterator it = find(myOmtf.begin(), myOmtf.end(), csctf);
     if (it == myOmtf.end() ) {
       std::cout << "HERE PROBLEM!!! ----- OMTF DIGI corresponding to CSCTF ("<<csctf<<") NOT FOUND! " << std::endl;
       hasError = true;
       omtfDigisError++; 
     }
  }
  std::cout <<" All OMTF: " << omtfDigis <<", with Error: "<< omtfDigisError <<" has Error: " << hasError << std::endl;

}
DEFINE_FWK_MODULE(OmtfDigiCompare);
