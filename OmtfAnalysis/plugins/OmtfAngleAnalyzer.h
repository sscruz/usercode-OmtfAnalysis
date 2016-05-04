#ifndef UserCode_OmtfAnalysis_OmtfAngleAnalyzer_H
#define UserCode_OmtfAnalysis_OmtfAngleAnalyzer_H

#include <string>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
class RPCReadOutMapping;

class OmtfAngleAnalyzer : public edm::EDAnalyzer {
public:
  OmtfAngleAnalyzer (const edm::ParameterSet & cfg);
  virtual ~OmtfAngleAnalyzer(){}
  virtual void beginJob();
  virtual void beginRun(const edm::Run&,  const edm::EventSetup& es);

  virtual void analyze(const edm::Event&, const edm::EventSetup& es) { }

private:
  std::string lbNameforDetId(const RPCReadOutMapping* cabling, uint32_t rawDetId);
  void csc2omtf( const edm::EventSetup& es);
  void rpc2omtf( const edm::EventSetup& es);
  void dt2omtf( const edm::EventSetup& es);
};

#endif
