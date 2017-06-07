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
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"

#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"



#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

    struct MyDigi { 
       uint32_t det; int data; int bx; 
       bool operator==(const MyDigi&o) const {
         return (det==o.det && data==o.data && bx==o.bx);
       } 
       bool operator< (const MyDigi&o) const { 
         if (this->det < o.det) return true;
         if (this->det > o.det) return false; 
         if (this->data < o.data) return true;
         if (this->data > o.data) return false;
         if (this->bx < o.bx) return true;
         return false;
       }
       friend std::ostream & operator<< (std::ostream &out, const MyDigi &o) {
         out<<"MyDigi det: "<<o.det<<", data: "<<o.data<<", bx: "<<o.bx; 
         return out;
       }
    };



class OmtfDigiCompare : public edm::EDAnalyzer {
public:
  OmtfDigiCompare (const edm::ParameterSet & cfg);
  virtual ~OmtfDigiCompare(){}

  virtual void analyze(const edm::Event &ev, const edm::EventSetup& es) {
    static int nEvents = 0;
/*
    if (    ev.id().event() != 483248 
        &&  ev.id().event() != 480262
        &&  ev.id().event() != 471470
        &&  ev.id().event() != 472951 ) return; 
*/
    std::cout <<"======> Analyze #"<<++nEvents<<", ev: "<<ev.id().event()<<std::endl;
    analyzeDT(ev,es);
//    analyzeCSC(ev,es);
//    analyzeRPC(ev,es);
//    analyzeOMTF(ev,es);
  }
  void analyzeCSC(const edm::Event&, const edm::EventSetup& es);
  void analyzeDT(const edm::Event&, const edm::EventSetup& es);
  void analyzeRPC(const edm::Event&, const edm::EventSetup& es);
  void analyzeOMTF(const edm::Event&, const edm::EventSetup& es);

private:
    edm::EDGetTokenT<RPCDigiCollection> inputRPC_PACT, inputRPC_OMTF;
    edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection> inputCSC_CSC, inputCSC_OMTF;
    edm::EDGetTokenT<L1MuDTChambPhContainer> inputDTPh_BMTF, inputDTPh_OMTF;
    edm::EDGetTokenT<L1MuDTChambThContainer> inputDTTh_BMTF, inputDTTh_OMTF;
    edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> inputOMTF_DATA, inputOMTF_EMUL; 

    unsigned int omtfDigis, omtfDigisError; 
};


OmtfDigiCompare::OmtfDigiCompare(const edm::ParameterSet & cfg) : omtfDigis(0), omtfDigisError(0) {
  inputRPC_PACT = consumes<RPCDigiCollection>(cfg.getParameter<edm::InputTag>("srcRPC_PACT"));
  inputRPC_OMTF = consumes<RPCDigiCollection>(cfg.getParameter<edm::InputTag>("srcRPC_OMTF"));
  inputCSC_CSC = consumes<CSCCorrelatedLCTDigiCollection>(cfg.getParameter<edm::InputTag>("srcCSC_CSC"));
  inputCSC_OMTF = consumes<CSCCorrelatedLCTDigiCollection>(cfg.getParameter<edm::InputTag>("srcCSC_OMTF"));
  inputDTPh_BMTF = consumes<L1MuDTChambPhContainer>(cfg.getParameter<edm::InputTag>("srcDTPh_BMTF"));
  inputDTTh_BMTF = consumes<L1MuDTChambThContainer>(cfg.getParameter<edm::InputTag>("srcDTTh_BMTF"));
  inputDTPh_OMTF = consumes<L1MuDTChambPhContainer>(cfg.getParameter<edm::InputTag>("srcDTPh_OMTF"));
  inputDTTh_OMTF = consumes<L1MuDTChambThContainer>(cfg.getParameter<edm::InputTag>("srcDTTh_OMTF"));
  inputOMTF_DATA = consumes<l1t::RegionalMuonCandBxCollection>(cfg.getParameter<edm::InputTag>("srcOMTF_DATA"));
  inputOMTF_EMUL = consumes<l1t::RegionalMuonCandBxCollection>(cfg.getParameter<edm::InputTag>("srcOMTF_EMUL"));
  
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
  std::vector<MyDigi> myBmtfPh;
  for (const auto &  chDigi : *dtphDigisBMTF.getContainer() ) {
    if (abs(chDigi.whNum()) != 2) continue;
    if (chDigi.stNum() ==4) continue;
    if (chDigi.bxNum() != 0) continue;
    if (chDigi.code()==7) continue;
    MyDigi myDigi = { DTChamberId(chDigi.whNum(),chDigi.stNum(),chDigi.scNum()+1).rawId(), chDigi.phi()+chDigi.phiB()+chDigi.code(), chDigi.bxNum() };
    if (myBmtfPh.end() == std::find(myBmtfPh.begin(), myBmtfPh.end(), myDigi)) myBmtfPh.push_back(myDigi);
    std::cout <<"DtDataWord64 BMTF    " 
        <<" bxNum: "<<chDigi.bxNum()
        <<" whNum: "<<chDigi.whNum()
        <<" station: "<< chDigi.stNum()
        <<" sector: "<< chDigi.scNum()
        <<" phi:   "<<chDigi.phi()
        <<" phiB:   "<<chDigi.phiB()
        <<" code(q): "<< chDigi.code()
        << std::endl;
  }
  std::sort(myBmtfPh.begin(), myBmtfPh.end());
  std::cout <<" myBmtfPh size is: " << myBmtfPh.size() << std::endl;

  std::vector<MyDigi> myBmtfTh;
  edm::Handle<L1MuDTChambThContainer> digiCollectionDTTh_BMTF;
  ev.getByToken(inputDTTh_BMTF, digiCollectionDTTh_BMTF);
  const L1MuDTChambThContainer& dtthDigisBMTF = *digiCollectionDTTh_BMTF.product();
  for (const auto &  chDigi : *dtthDigisBMTF.getContainer() ) {
    unsigned int eta = 0;
    unsigned int etaQ = 0;
    for (unsigned int ipos=0; ipos <7; ipos++) {
     if (chDigi.position(ipos) >1 ) std::cout <<" HERE PROBLEM !!!!" << std::endl;
     if (chDigi.position(ipos)==1) eta |= (1 <<ipos);
     if (chDigi.quality(ipos)==1) etaQ |= (1 <<ipos);
    }
    if (chDigi.bxNum() != 0) continue;
    if (!etaQ) continue;
    if (abs(chDigi.whNum()) != 2) continue;
    MyDigi myDigi = { DTChamberId(chDigi.whNum(),chDigi.stNum(),chDigi.scNum()+1).rawId(), (int)(eta+etaQ), chDigi.bxNum() };
    if (myBmtfTh.end() == std::find(myBmtfTh.begin(), myBmtfTh.end(), myDigi)) myBmtfTh.push_back(myDigi);
    std::cout <<"DtDataWord64 BMTF TH " 
        <<" bxNum: "<<chDigi.bxNum()
        <<" whNum: "<<chDigi.whNum()
        <<" station: "<< chDigi.stNum()
        <<" sector: "<< chDigi.scNum()
        <<" eta: " << eta
        <<" etaQ: " << etaQ
        << std::endl;
  }
  std::sort(myBmtfTh.begin(), myBmtfTh.end());
  std::cout <<" myBmtfTh size is: " << myBmtfTh.size() << std::endl;

  edm::Handle<L1MuDTChambPhContainer> digiCollectionDTPh_OMTF;
  ev.getByToken(inputDTPh_OMTF, digiCollectionDTPh_OMTF);
  const L1MuDTChambPhContainer& dtphDigisOMTF = *digiCollectionDTPh_OMTF.product();
  std::cout <<" DTPh digis from OMTF " << dtphDigisOMTF.getContainer()->size()<< std::endl;
  std::vector<MyDigi> myOmtfPh;
  for (const auto &  chDigi : *dtphDigisOMTF.getContainer() ) {
    if (chDigi.bxNum() != 0) continue;
    MyDigi myDigi = { DTChamberId(chDigi.whNum(),chDigi.stNum(),chDigi.scNum()+1).rawId(), chDigi.phi()+chDigi.phiB()+chDigi.code(), chDigi.bxNum() };
    if (myOmtfPh.end() == std::find(myOmtfPh.begin(), myOmtfPh.end(), myDigi)) myOmtfPh.push_back(myDigi);
    std::cout <<"DtDataWord64 OMTF    " 
        <<" bxNum: "<<chDigi.bxNum()
        <<" whNum: "<<chDigi.whNum()
        <<" station: "<< chDigi.stNum()
        <<" sector: "<< chDigi.scNum()
        <<" phi:   "<<chDigi.phi()
        <<" code(q): "<< chDigi.code()
        << std::endl;
  }
  std::sort(myOmtfPh.begin(), myOmtfPh.end());
  std::cout <<" myOmtfPh size is: " << myOmtfPh.size() << std::endl;

  edm::Handle<L1MuDTChambThContainer> digiCollectionDTTh_OMTF;
  ev.getByToken(inputDTTh_OMTF, digiCollectionDTTh_OMTF);
  const L1MuDTChambThContainer& dtthDigisOMTF = *digiCollectionDTTh_OMTF.product();
  std::vector<MyDigi> myOmtfTh;
  for (const auto &  chDigi : *dtthDigisOMTF.getContainer() ) {
    if (chDigi.bxNum() != 0) continue;
    unsigned int eta = 0;
    unsigned int etaQ = 0;
    for (unsigned int ipos=0; ipos <7; ipos++) {
     if (chDigi.position(ipos)==1) eta |= (1 <<ipos);
     if (chDigi.quality(ipos)==1) etaQ |= (1 <<ipos);
    }
    MyDigi myDigi = { DTChamberId(chDigi.whNum(),chDigi.stNum(),chDigi.scNum()+1).rawId(), (int)(eta+etaQ), chDigi.bxNum() };
    if (myOmtfTh.end() == std::find(myOmtfTh.begin(), myOmtfTh.end(), myDigi)) myOmtfTh.push_back(myDigi);

    std::cout <<"DtDataWord64 OMTF TH " 
        <<" bxNum: "<<chDigi.bxNum()
        <<" whNum: "<<chDigi.whNum()
        <<" station: "<< chDigi.stNum()
        <<" sector: "<< chDigi.scNum()
        <<" eta: " << eta
        <<" etaQ: " << etaQ
        << std::endl;
  }
  std::sort(myOmtfTh.begin(), myOmtfTh.end());
  std::cout <<" myOmtfTh size is: " << myOmtfTh.size() << std::endl;

  bool hasError = false;
  for (const auto & omtf : myOmtfPh ) {
     omtfDigis++;     
     std::vector<MyDigi>::const_iterator itCsc = find(myBmtfPh.begin(), myBmtfPh.end(), omtf);
     if (itCsc == myBmtfPh.end() ) {
       std::cout << "HERE PROBLEM!!! ----- BMTF_PH DIGI corresponding to OMTF ("<<omtf<<") NOT FOUND! " << std::endl;
       hasError = true;
       omtfDigisError++; 
     }
  }
  for (const auto & other : myBmtfPh ) {
     omtfDigis++;     
     std::vector<MyDigi>::const_iterator it = find(myOmtfPh.begin(), myOmtfPh.end(), other);
     if (it == myOmtfPh.end() ) {
       std::cout << "HERE PROBLEM!!! ----- OMTF_PH DIGI corresponding to BMTF("<<other<<") NOT FOUND! " << std::endl;
       hasError = true;
       omtfDigisError++; 
     }
  }
/*
  for (const auto & omtf : myOmtfTh ) {
     omtfDigis++;     
     std::vector<MyDigi>::const_iterator itOth = find(myBmtfTh.begin(), myBmtfTh.end(), omtf);
     if (itOth == myBmtfTh.end() ) {
       std::cout << "HERE PROBLEM!!! ----- BMTF_TH DIGI corresponding to OMTF ("<<omtf<<") NOT FOUND! " << std::endl;
       hasError = true;
       omtfDigisError++; 
     }
  }
  for (const auto & other : myBmtfTh ) {
     omtfDigis++;     
     std::vector<MyDigi>::const_iterator it = find(myOmtfTh.begin(), myOmtfTh.end(), other);
     if (it == myOmtfTh.end() ) {
       std::cout << "HERE PROBLEM!!! ----- OMTF_TH DIGI corresponding to BMTF("<<other<<") NOT FOUND! " << std::endl;
       hasError = true;
       omtfDigisError++; 
     }
  }
*/
  std::cout <<" All OMTF: " << omtfDigis <<", with Error: "<< omtfDigisError <<" has Error: " << hasError << std::endl;
  
}

void OmtfDigiCompare::analyzeOMTF( const edm::Event &ev, const edm::EventSetup& es) {
  std::cout << "-------- HERE DIGI COMPARE OMTF---------" << std::endl;
  int bxNumber = 0;

  edm::Handle<l1t::RegionalMuonCandBxCollection> digiCollectionOMTF_DATA;
  ev.getByToken(inputOMTF_DATA,digiCollectionOMTF_DATA);
  std::cout <<" OMTF digis from DATA" << std::endl;
  std::vector<MyDigi> myData;
  for (l1t::RegionalMuonCandBxCollection::const_iterator it = digiCollectionOMTF_DATA.product()->begin(bxNumber);
       it != digiCollectionOMTF_DATA.product()->end(bxNumber); ++it) {
    //MyDigi myDigi = { (unsigned int)it->processor(),  (int)it->hwPhi()+(int)it->hwEta()+(int)it->hwPt(), bxNumber};
    MyDigi myDigi = { (unsigned int)it->processor(),  it->hwPt(), bxNumber};
    std::cout <<"PT: "<<it->hwPt()<<" ETA: "<<it->hwEta()<<" PHI: "<<it->hwPhi()<< std::endl;
    std::cout <<" MyDigi (DATA): " << myDigi << std::endl;
    if (myData.end() == std::find(myData.begin(), myData.end(), myDigi)) myData.push_back(myDigi);
    else std::cout <<" DUPLICATE. " << std::endl;
  }
  std::sort(myData.begin(),myData.end());
  std::cout <<" myData size is: " << myData.size() << std::endl;

  edm::Handle<l1t::RegionalMuonCandBxCollection> digiCollectionOMTF_EMUL;
  ev.getByToken(inputOMTF_EMUL,digiCollectionOMTF_EMUL);
  std::cout <<" OMTF digis from EMUL" << std::endl;
  std::vector<MyDigi> myEmul;
  for (l1t::RegionalMuonCandBxCollection::const_iterator it = digiCollectionOMTF_EMUL.product()->begin(bxNumber);
       it != digiCollectionOMTF_EMUL.product()->end(bxNumber); ++it) {
    // MyDigi myDigi = { (unsigned int)it->processor(),  )it->hwPhi()+(int)it->hwEta()+(int)it->hwPt(), bxNumber};
    std::cout <<"PT: "<<it->hwPt()<<" ETA: "<<it->hwEta()<<" PHI: "<<it->hwPhi()<< std::endl;
    MyDigi myDigi = { (unsigned int)it->processor(),  it->hwPt(), bxNumber};
    std::cout <<" MyDigi (EMUL): " << myDigi << std::endl;
    if (myEmul.end() == std::find(myEmul.begin(), myEmul.end(), myDigi)) myEmul.push_back(myDigi);
    else std::cout <<" DUPLICATE. " << std::endl;
  }
  std::sort(myEmul.begin(),myEmul.end());
  std::cout <<" myEmul size is: " << myEmul.size() << std::endl;
 
  bool hasError = false;
  for (const auto & omtf : myData ) {
     omtfDigis++;     
     std::vector<MyDigi>::const_iterator it = find(myEmul.begin(), myEmul.end(), omtf);
     if (it == myEmul.end() ) {
       std::cout << "HERE PROBLEM!!! ----- OMTF EMUL DIGI corresponding to OMTF DATA("<<omtf<<") NOT FOUND! " << std::endl;
       hasError = true;
       omtfDigisError++; 
     }
  }
  for (const auto & other : myEmul) {
     omtfDigis++;     
     std::vector<MyDigi>::const_iterator it = find(myData.begin(), myData.end(), other);
     if (it == myData.end() ) {
       std::cout << "HERE PROBLEM!!! ----- OMTF DATA DIGI corresponding to OMTF EMUL("<<other<<") NOT FOUND! " << std::endl;
       hasError = true;
       omtfDigisError++; 
     }
  }
  std::cout <<" All OMTF: " << omtfDigis <<", with Error: "<< omtfDigisError <<" has Error: " << hasError << std::endl;
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
      MyDigi myDigi = { rawId, digi->getStrip(), digi->getBX() };
      std::cout <<" MyDigi (OMTF): " << myDigi << std::endl;
      if (myOmtf.end() == std::find(myOmtf.begin(), myOmtf.end(), myDigi)) myOmtf.push_back(myDigi);
      else std::cout <<" DUPLICATE. " << std::endl;
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
      std::cout <<" MyDigi (CSCTF): " << myDigi << std::endl;
      if (myCsc.end() == std::find(myCsc.begin(), myCsc.end(), myDigi)) myCsc.push_back(myDigi);
    }
  } 
  std::sort(myCsc.begin(),myCsc.end());
  std::cout <<" myCsc size is: " << myCsc.size() << std::endl;

  bool hasError = false;
  for (const auto & omtf : myOmtf ) {
     if (omtf.bx != 6) continue;
     omtfDigis++;     
     std::vector<MyDigi>::const_iterator itCsc = find(myCsc.begin(), myCsc.end(), omtf);
     if (itCsc == myCsc.end() ) {
       std::cout << "HERE PROBLEM!!! ----- CSC DIGI corresponding to OMTF ("<<omtf<<") NOT FOUND! " << std::endl;
       hasError = true;
       omtfDigisError++; 
     }
  }
  for (const auto & csctf : myCsc ) {
     if (csctf.bx != 6) continue;
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
