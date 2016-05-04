#ifndef UserCode_OmtfAnalysis_OmtfLocalDaqAnalyzer_H
#define UserCode_OmtfAnalysis_OmtfLocalDaqAnalyzer_H

#include "TObjArray.h"
#include "UserCode/OmtfAnalysis/interface/OmtfAlgoHit.h"


#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class OmtfLocalDaqAnalyzer : public edm::EDAnalyzer {

struct OmtfData {
  int number;
  std::vector<OmtfAlgoHit> hits;
  std::vector<OmtfAlgoMuon> muons;
  OmtfGmtData  gmt;
  friend std::ostream & operator<< (std::ostream &out, const OmtfData &o) {
    out << "Event: "<<o.number<<std::endl;
    for (auto ih=o.hits.begin(); ih != o.hits.end(); ih++) out <<"  "<< (*ih) << std::endl;
    for (auto im=o.muons.begin(); im != o.muons.end(); im++) out <<"  "<< (*im) << std::endl;
    for (auto ig=o.gmt.muons.begin(); ig != o.gmt.muons.end(); ig++) { out <<ig-o.gmt.muons.begin()<<"_"<<(*ig) << std::endl; }
    return out;
  }
};

public:
  OmtfLocalDaqAnalyzer (const edm::ParameterSet & cfg);
  virtual ~OmtfLocalDaqAnalyzer(){}
  virtual void beginJob() { std::cout << "begin Job" << std::endl; }
  virtual void beginRun(const edm::Run&,  const edm::EventSetup& es) {
    std::cout <<"beingRun " << std::endl;
  }

  virtual void analyze(const edm::Event&, const edm::EventSetup& es);

private:
  void processLastEvent();

private:
  std::string theInFileName;
  OmtfData theEvent;
  TObjArray theHistos;

};

#endif
