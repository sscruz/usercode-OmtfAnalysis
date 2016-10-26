// system include files
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>
#include <map>
#include <string>

// user include files
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"



#include "DataFormats/FEDRawData/interface/FEDHeader.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDTrailer.h"
#include "DataFormats/FEDRawData/interface/FEDRawData.h"

#include "EventFilter/L1TRawToDigi/interface/AMC13Spec.h"
#include "EventFilter/L1TRawToDigi/interface/AMCSpec.h"

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

typedef uint64_t Word64;

class RPCDataWord64 {
public: 
  RPCDataWord64(Word64 data=0) : rawData(data) {}
  unsigned int bxNum() const { return bxNum_; }
  unsigned int frame1() const { return frame1_;}
  unsigned int frame2() const { return frame2_;}
  unsigned int frame3() const { return frame3_;}
  unsigned int empty() const { return empty_;}
  unsigned int linkNum() const { return linkNum_;}
  unsigned int type() const { return type_;}
  friend std::ostream & operator<< (std::ostream &out, const RPCDataWord64 &o) {
    out << "RPCDataWord64: "
        <<" type: "<< std::bitset<4>(o.type_) 
        << " bx: "<<o.bxNum_
        << " lnk: "<< o.linkNum_;
    out << std::hex;
    out << " frame1: 0x"<< o.frame1_; if (o.frame1_ != 0) out <<" ("<< std::bitset<16>(o.frame1_)<<")";
    out << " frame2: 0x"<< o.frame2_; if (o.frame2_ != 0) out <<" ("<< std::bitset<16>(o.frame2_)<<")";
    out << " frame3: 0x"<< o.frame3_; if (o.frame3_ != 0) out <<" ("<< std::bitset<16>(o.frame3_)<<")";
    out << std::dec;
    return out;
  }
private:
  union {
    uint64_t rawData;
    struct {
      uint64_t frame3_  : 16;
      uint64_t frame2_  : 16;
      uint64_t frame1_  : 16;
      uint64_t empty_   : 4;
      uint64_t bxNum_   : 3;
      uint64_t linkNum_ : 5;
      uint64_t type_    : 4;
    };
  };
};


class OmtfEleIndex {
public:
  OmtfEleIndex() : packed_(0) {}
  OmtfEleIndex(const std::string & board, unsigned int link) {
    unsigned int fed = 0;
    if (board.substr(4,1)=="n") fed = 1380; else if (board.substr(4,1)=="p") fed = 1381;
    unsigned int amc = std::stoi( board.substr(5,1) );
    packed_ = fed*1000+amc*100+link; 
  }
  OmtfEleIndex(unsigned int fed, unsigned int amc, unsigned int link) { packed_ = fed*1000+amc*100+link; }
  unsigned int fed() const  { return packed_/1000; }
  unsigned int amc() const  { return ( (packed_ /100) %10); }
  unsigned int link() const { return packed_ % 100; } 
  friend std::ostream & operator<< (std::ostream &out, const OmtfEleIndex&o) {
    out << "OMTF";
    if (o.fed()==1380) out <<"n";
    if (o.fed()==1381) out <<"p";
    out << o.amc();
    out <<" (fed: "<<o.fed()<<"), ln: " << o.link();
    return out;
  }
  inline bool operator< (const OmtfEleIndex& o) const { return this->packed_ < o.packed_; }

private:
  uint32_t packed_;

};


class RpcLinkMap {
public:
  RpcLinkMap() { }
  void init( const std::string& file, std::string path ) {
    std::ifstream inFile;
    std::string fName = path + "/" + file;
    inFile.open(fName);
    if (inFile) {
      LogTrace("")<<" reading OmtfRpcLinksMap from: "<<fName;
    } else {
      LogTrace("")<<" Unable to open file "<<fName;

     throw std::runtime_error("Unable to open OmtfRpcLinksMap file " + fName);
    }

    std::string line;
    while (std::getline(inFile, line)) {
      line.erase(0, line.find_first_not_of(" \t\r\n"));      //cut first character
      if (line.empty() || !line.compare(0,2,"--")) continue; // empty or comment line
      std::stringstream ss(line);
      std::string processorName, lbName;
      unsigned int link, dbId;
      if (ss >> processorName >> link >> lbName >> dbId) {
          std::map< unsigned int, std::string > & li2lb = link2lbName[processorName];
          std::map< std::string, unsigned int > & lb2li = lbName2link[processorName];
          li2lb[link] = lbName;
          lb2li[lbName] = link;
          OmtfEleIndex ele(processorName, link);
          lbName2OmtfIndex[lbName].push_back(ele);
//          std::cout <<"read: " <<processorName<<" "<<link<<" "<<lbName<<" Ele: "<< ele <<" map Size: "<< lbName2OmtfIndex.size() <<std::endl;
      }
    }
//    std::cout <<" size: " << link2lbName.size() <<" "<<lbName2link.size() << " " << lbName2OmtfIndex.size()<< std::endl;
    inFile.close();
  }

  const std::string & lbName(const std::string& board, unsigned int link) const {
    return link2lbName.at(board).at(link);
  }

  unsigned int        link(const std::string& board, const std::string& lbName) const {
    return lbName2link.at(board).at(lbName);
  }

  const std::vector<OmtfEleIndex> & omtfEleIndex ( const std::string& lbName) const {
     return lbName2OmtfIndex.at(lbName);
  }

private:
    std::map<std::string, std::map<unsigned int, std::string> > link2lbName; //[processorName][rpcRxNum] - lbName
    std::map<std::string, std::map<std::string, unsigned int> > lbName2link; //[processorName][lbName] - rpcRxNum
    std::map<std::string, std::vector<OmtfEleIndex> > lbName2OmtfIndex; //[lbName] - vector of {board,rpcRxNum}

};


class OmtfUnpacker: public edm::stream::EDProducer<> {
public:

    ///Constructor
    OmtfUnpacker(const edm::ParameterSet& pset);

    ~OmtfUnpacker() {}

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    void produce(edm::Event & ev, const edm::EventSetup& es) override;

    void beginRun(const edm::Run &run, const edm::EventSetup& es) override;

private:

  edm::InputTag dataLabel_;
  unsigned long eventCounter_;

  edm::EDGetTokenT<FEDRawDataCollection> fedToken_;

  std::map<OmtfEleIndex, LinkBoardElectronicIndex> omtf2rpc_;
  
  const RPCReadOutMapping* theCabling;



};

OmtfUnpacker::OmtfUnpacker(const edm::ParameterSet& pset)
{
  produces<RPCDigiCollection>("OMTF");
  fedToken_ = consumes<FEDRawDataCollection>(pset.getParameter<edm::InputTag>("InputLabel"));
}

void OmtfUnpacker::beginRun(const edm::Run &run, const edm::EventSetup& es) {
  edm::ESTransientHandle<RPCEMap> readoutMapping;
  es.get<RPCEMapRcd>().get(readoutMapping);
  const RPCReadOutMapping * cabling= readoutMapping->convert();
  theCabling = cabling;
  std::cout <<" Has readout map, VERSION: " << cabling->version() << std::endl;

  RpcLinkMap omtfLink2Ele;
  omtfLink2Ele.init("OmtfRpcLinksMap.txt",".");

  std::vector<const DccSpec*> dccs = cabling->dccList();
  for (std::vector<const DccSpec*>::const_iterator it1= dccs.begin(); it1!= dccs.end(); ++it1) {
    const std::vector<TriggerBoardSpec> & rmbs = (*it1)->triggerBoards();
    for (std::vector<TriggerBoardSpec>::const_iterator it2 = rmbs.begin(); it2 != rmbs.end(); ++it2) {
      const  std::vector<LinkConnSpec> & links = it2->linkConns();
      for (std::vector<LinkConnSpec>::const_iterator it3 = links.begin(); it3 != links.end(); ++it3) {
        const  std::vector<LinkBoardSpec> & lbs = it3->linkBoards();
        for (std::vector<LinkBoardSpec>::const_iterator it4=lbs.begin(); it4 != lbs.end(); ++it4) {
          
          try {
            std::string lbNameCH = it4->linkBoardName();
            std::string lbName = lbNameCH.substr(0,lbNameCH.size()-4);
            const std::vector<OmtfEleIndex> & omtfEles = omtfLink2Ele.omtfEleIndex(lbName); 
//            std::cout <<"  isOK ! " <<  it4->linkBoardName() <<" has: " << omtfEles.size() << " first: "<< omtfEles[0] << std::endl;
            LinkBoardElectronicIndex rpcEle = { (*it1)->id(), it2->dccInputChannelNum(), it3->triggerBoardInputNumber(), it4->linkBoardNumInLink()};
            for ( const auto & omtfEle : omtfEles ) omtf2rpc_[omtfEle]= rpcEle; 
          } 
          catch(...) { ; } // std::cout << "exception! "<<it4->linkBoardName()<< std::endl; }
        }
      }
    }
  }

  std::cout << " SIZE OF OMTF to RPC map  is: " << omtf2rpc_.size() << std::endl;

}

void OmtfUnpacker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("InputLabel",edm::InputTag("rawDataCollector"));
  descriptions.add("omtfUnpacker",desc);
}

void OmtfUnpacker::produce(edm::Event& event, const edm::EventSetup& setup)
{
//  std::cout << " HERE PRODUCE!" << std::endl;
  bool debug = edm::MessageDrop::instance()->debugEnabled;
  eventCounter_++;
  if (debug) LogDebug ("OmtfUnpacker::produce") <<"Beginning To Unpack Event: "<<eventCounter_;

  edm::Handle<FEDRawDataCollection> allFEDRawData;
  event.getByToken(fedToken_,allFEDRawData);

  std::auto_ptr<RPCDigiCollection> producedRPCDigis(new RPCDigiCollection);

  for (int fedId= 1380; fedId<= 1381; ++fedId){


    const FEDRawData & rawData = allFEDRawData->FEDData(fedId);
    int nWords = rawData.size()/sizeof(Word64);
    LogTrace("") <<"FED : " << fedId <<" words: " << nWords;
    if (nWords==0) continue;

    int triggerBX =0;
   
    //
    // FED header
    // Expect one FED header + AMC13 headers.
    const Word64* header = reinterpret_cast<const Word64* >(rawData.data());
    FEDHeader fedHeader( reinterpret_cast<const unsigned char*>(header));
    if (!fedHeader.check()) { LogTrace("") <<" ** PROBLEM **, header.check() failed, break"; break; }
    if ( fedHeader.sourceID() != fedId) {
      LogTrace("") <<" ** PROBLEM **, fedHeader.sourceID() != fedId"
          << "fedId = " << fedId<<" sourceID="<<fedHeader.sourceID();
    }
    triggerBX = fedHeader.bxID();
    if (debug) {
      std::ostringstream str;
      str <<"  header: "<< *reinterpret_cast<const std::bitset<64>*> (header) << std::endl;
      str <<"  header triggerType: " << fedHeader.triggerType()<< std::endl;
      str <<"  header lvl1ID:      " << fedHeader.lvl1ID() << std::endl;
      str <<"  header bxID:        " << fedHeader.bxID() << std::endl;
      str <<"  header sourceID:    " << fedHeader.sourceID() << std::endl;
      str <<"  header version:     " << fedHeader.version() << std::endl;
      str <<"  header more :     " << fedHeader.moreHeaders() << std::endl;
      str << " triggerBx "<< triggerBX << std::endl;
      LogTrace("") << str.str();
    }
    // AMC13 headers


    // 
    // FED trailed
    //
    const Word64* trailer=reinterpret_cast<const Word64* >(rawData.data())+(nWords-1); trailer++;
    bool moreTrailers = true;
    while (moreTrailers) {
      trailer--;
      FEDTrailer fedTrailer(reinterpret_cast<const unsigned char*>(trailer));
      if ( !fedTrailer.check()) {
        if (debug) LogTrace("") <<" ** PROBLEM **, trailer.check() failed, break";
        break;
      }
      if ( fedTrailer.lenght()!= nWords) {
        if (debug) LogTrace("")<<" ** PROBLEM **, fedTrailer.lenght()!= nWords, break";
        break;
      }
      moreTrailers = fedTrailer.moreTrailers();
      if (debug) {
        std::ostringstream str;
        str <<" trailer: "<<  *reinterpret_cast<const std::bitset<64>*> (trailer) << std::endl;
        str <<"  trailer lenght:    "<<fedTrailer.lenght()<< std::endl;
        str <<"  trailer crc:       "<<fedTrailer.crc()<< std::endl;
        str <<"  trailer evtStatus: "<<fedTrailer.evtStatus()<< std::endl;
        str <<"  trailer ttsBits:   "<<fedTrailer.ttsBits()<< std::endl;
        LogTrace("") << str.str();
      }
    }

    if (debug) {
      std::ostringstream str;
      for (const Word64* word = header+1; word != trailer; word++) {
        str<<"    data: "<<*reinterpret_cast<const std::bitset<64>*>(word) << std::endl;
      }
      LogTrace("") << str.str();
    }

    const Word64* headerAmc13raw = header+1;
    amc13::Header headerAmc13(headerAmc13raw);
    if (debug) {
      std::ostringstream str;
      str <<" headerAMC13:  "<<  *reinterpret_cast<const std::bitset<64>*> (headerAmc13raw) << std::endl;
      str <<" amc13 check:  "<< headerAmc13.check() << std::endl;
      str <<" amc13 format: "<< headerAmc13.getFormatVersion() << std::endl;
      str <<" amc13 nAMCs:  "<< headerAmc13.getNumberOfAMCs() << std::endl;
      str <<" amc13 orbit:  "<< headerAmc13.getOrbitNumber() << std::endl;
//      LogTrace("") << str.str();
    }


    //unsigned int nAMCs = headerAmc13.getNumberOfAMCs();
    //for (unsigned int iAMC = 0; iAMC <nAMCs; iAMC++) {
    // const Word64* raw = header+1 +(iAMC+1);
    //}
   
    const Word64* trailerAmc13raw = trailer-1;
    amc13::Trailer trailerAmc13(trailerAmc13raw);
    if (debug) {
      std::ostringstream str;
      str <<" trailerAMC13:  "<<  *reinterpret_cast<const std::bitset<64>*> (trailerAmc13raw) << std::endl;
      str <<" crc:  "<< trailerAmc13.getCRC() << std::endl;
      str <<" block: "<< trailerAmc13.getBlock() << std::endl;
      str <<" LV1ID:  "<< trailerAmc13.getLV1ID() << std::endl;
      str <<" BX:  "<< trailerAmc13.getBX() << std::endl;
//      LogTrace("") << str.str();
    }

    amc13::Packet packetAmc13;
    if (!packetAmc13.parse( header, header+1, nWords-2, fedHeader.lvl1ID(), fedHeader.bxID(), 1, 0)) {
      edm::LogError("OMTF") << "Could not extract AMC13 Packet.";
      return;
    } 
//    std::cout <<"AMC13 Packet: "<< packetAmc13.blocks() << " size "<<packetAmc13.size() << std::endl;  
    for (auto amc: packetAmc13.payload()) {
//      amc.finalize(fedHeader.lvl1ID(), fedHeader.bxID(), true, false);
      amc::BlockHeader bh =  amc.blockHeader(); 
      if (debug) {
        std::ostringstream str;
        str <<" blockheader:  "<<  std::bitset<64>(bh.raw()) << std::endl;
        str <<" boardID:  "<< bh.getBoardID() << std::endl;
        str <<" amcNumber:  "<< bh.getAMCNumber() << std::endl;
        str <<" size:  "<< bh.getSize() << std::endl;
//        LogTrace("") << str.str();
      }

      //
      // AMC header
      //
      amc::Header headerAmc = amc.header(); 
      if (debug) {
        std::ostringstream str;
        str <<" AMC header[0]:  "<<  std::bitset<64>(headerAmc.raw()[0]) << std::endl;
        str <<" AMC header[1]:  "<<  std::bitset<64>(headerAmc.raw()[1]) << std::endl;
        str <<" AMC number:     "<< headerAmc.getAMCNumber() << std::endl;
//        LogTrace("") << str.str();
      }


      //
      //
      //
      //amc::Trailer trailerAmc = amc.trailer(); //this is the expected way bud does not work 
      amc::Trailer trailerAmc(amc.data().get()+amc.size()-1); //FIXME: the above is prefered
      if (debug) {
        std::ostringstream str;
        str <<" AMC trailer:  "<<  std::bitset<64>(trailerAmc.raw()) << std::endl;
        str <<" getLV1ID:     "<< trailerAmc.getLV1ID() << std::endl;
        str <<" size:         "<< trailerAmc.getSize() << std::endl;
//        LogTrace("") << str.str();
      }
      auto payload64 = amc.data();
      const Word64* word = payload64.get();
      for (unsigned int iWord= 1; iWord<= amc.size(); iWord++, word++) {
        if (iWord<=2 ) continue; // two header words for each AMC
        if (iWord==amc.size() ) continue; // trailer for each AMC 
        LogTrace("") <<" payload: " <<  *reinterpret_cast<const std::bitset<64>*>(word);
        RPCDataWord64 data(*word);
        LogTrace("") <<" DATA : " << data;

        OmtfEleIndex omtfEle(fedHeader.sourceID(), bh.getAMCNumber()/2+1, data.linkNum());
//        std::cout << " OmtfEleIndex is: " << omtfEle << std::endl;
        LinkBoardElectronicIndex rpcEle = omtf2rpc_.at(omtfEle);
//        std::cout <<" rpcEle FED: " << rpcEle.dccId << std::endl; 
        RPCRecordFormatter formater(rpcEle.dccId, theCabling);


        rpcrawtodigi::EventRecords records(triggerBX);
        rpcrawtodigi::RecordBX recordBX(triggerBX+data.bxNum()-2);
        records.add(recordBX);   // warning: event records must be added in right order
        rpcrawtodigi::RecordSLD recordSLD(rpcEle.tbLinkInputNum, rpcEle.dccInputChannelNum);
        records.add(recordSLD); // warning: event records must be added in right order

        for (unsigned int iframe=1; iframe <=3; iframe++) {

          uint16_t frame = (iframe==1) ?  data.frame1() : ( (iframe==2) ?  data.frame2() : data.frame3() );
          if (frame==0) continue;
          rpcrawtodigi::RecordCD recordCD(frame);
          records.add(recordCD);

          LogTrace("") <<"OMTF->RPC Event isComplete: "<<records.complete() <<records.print(rpcrawtodigi::DataRecord::StartOfBXData); // << std::endl; 
          LogTrace("") <<"OMTF->RPC Event:             "<<records.print(rpcrawtodigi::DataRecord::StartOfTbLinkInputNumberData) << std::endl; 
          LogTrace("") <<"OMTF->RPC Event:             "<<records.print(rpcrawtodigi::DataRecord::ChamberData)
                    <<" lb:"<< recordCD.lbInLink() 
                    <<" part: "<< recordCD.partitionNumber() 
                    <<" partData: "<<recordCD.partitionData() 
                    << std::endl;

          if (records.complete()) formater.recordUnpack( records,  producedRPCDigis.get(), 0,0);
        }
      }
    }         

  } 
  event.put(producedRPCDigis,"OMTF");


}

DEFINE_FWK_MODULE(OmtfUnpacker);
