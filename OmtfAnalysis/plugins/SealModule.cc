#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"



#include "OmtfLocalDaqAnalyzer.h"
DEFINE_FWK_MODULE(OmtfLocalDaqAnalyzer); 

#include "OmtfAngleAnalyzer.h"
DEFINE_FWK_MODULE(OmtfAngleAnalyzer);

#include "UserCode/OmtfAnalysis/interface/OmtfTreeMaker.h"
DEFINE_FWK_MODULE(OmtfTreeMaker);

#include "OmtfTreeAnalysis.h"
DEFINE_FWK_MODULE(OmtfTreeAnalysis);

