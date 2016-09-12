#include "UserCode/OmtfAnalysis/interface/MenuInspector.h"

#include "FWCore/Framework/interface/Event.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
/*
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTExtendedCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
*/
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenuFwd.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerRecord.h"

#include "UserCode/OmtfDataFormats/interface/MuonObjColl.h"
#include "DataFormats/Math/interface/deltaR.h"



#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <bitset>

#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"
#include "CondFormats/DataRecord/interface/L1TUtmTriggerMenuRcd.h"
#include "CondFormats/L1TObjects/interface/L1TUtmTriggerMenu.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"

#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"


namespace {
  edm::EDGetTokenT<edm::TriggerResults> theTrigResultToken;
  edm::EDGetTokenT<GlobalAlgBlkBxCollection> theGlobalAlgToken;
}


MenuInspector::MenuInspector(const edm::ParameterSet& cfg, edm::ConsumesCollector&& cColl)
  : lastEvent(0), lastRun(0),
    theCounterIN(0), theCounterL1(0), theCounterHLT(0),
    theWarnNoColl(cfg.getUntrackedParameter<bool>("warnNoColl",true))
{ 
  theTrigResultToken = cColl.consumes<edm::TriggerResults>(edm::InputTag("TriggerResults","","HLT"));
  theGlobalAlgToken  = cColl.consumes<GlobalAlgBlkBxCollection>(edm::InputTag("gtStage2Digis"));

  cColl.consumes<trigger::TriggerEvent>(edm::InputTag("hltTriggerSummaryAOD","","HLT"));

  std::vector<std::string> names = cfg.getParameter<std::vector<std::string> >("namesCheckHltMuMatch");
  for (auto name : names) theNamesCheckHltMuMatchIdx[name]=-1;
}

bool MenuInspector::checkRun(const edm::Run& run, const edm::EventSetup & es)
{
  //
  // L1
  //
  edm::ESHandle<L1TUtmTriggerMenu> menu;
  es.get<L1TUtmTriggerMenuRcd>().get(menu);
  theNamesAlgoL1.clear();
  theNamesAlgoL1.resize(menu->getAlgorithmMap().size(),"");
  for (auto const & keyval: menu->getAlgorithmMap()) {
    std::string const & name  = keyval.second.getName();
    unsigned int        index = keyval.second.getIndex();
//    std::cout << " Index: " << index << " name: " << name << std::endl;
    if (index >= theNamesAlgoL1.size() ) theNamesAlgoL1.resize( index+1,"");  
    theNamesAlgoL1[index]=name;
  }

  //
  // HLT
  //
  bool changed(true);
  theTriggerParSetID = theHltConfig.processPSet().id(); 
  if (theHltConfig.init(run,es,"HLT",changed)) {
    if (changed) {
//      theHltConfig.dump("Streams");
//      theHltConfig.dump("Datasets");
//      theHltConfig.dump("Triggers");
//      theHltConfig.dump("PrescaleTable");
//      theHltConfig.dump("ProcessPSet");
      theNamesAlgoHLT.clear();
      for (auto & im : theNamesCheckHltMuMatchIdx) im.second = -1;

      //for goes up to .size()-1, since the last is "Final" decision.
      for (unsigned int idx =0;  idx < theHltConfig.size()-1; idx++) {
        std::string name = theHltConfig.triggerName(idx);
        theNamesAlgoHLT.push_back( name );
        for (auto & im : theNamesCheckHltMuMatchIdx) if (name.find(im.first) != std::string::npos) im.second = idx; 
      }
    }
  } 
  return true; 
}

MenuInspector::~MenuInspector()
{
  std::cout <<"MenuInspector, IN: "<<theCounterIN<<"  AfterL1: "<< theCounterL1 << " AfterHLT: "<<theCounterHLT<< std::endl;
}

bool MenuInspector::run(const edm::Event &ev, const edm::EventSetup &es)
{
  if (lastEvent==ev.id().event() && lastRun==ev.run()) return false;
  lastEvent = ev.id().event() ;
  lastRun = ev.run();

  theFiredL1 = runFiredAlgosL1(ev,es);
  theFiredHLT = runFiredAlgosHLT(ev,es);

  return true;
}

bool MenuInspector::associateHLT(const edm::Event &ev, const edm::EventSetup &es, MuonObjColl * muonColl)
{
  run(ev,es);

  //get HLT result
  edm::Handle<trigger::TriggerEvent> triggerAOD;
  ev.getByLabel(edm::InputTag("hltTriggerSummaryAOD","","HLT"), triggerAOD);
  if (!triggerAOD.isValid()) return false;
  const trigger::TriggerObjectCollection & triggerObjects = triggerAOD->getObjects();
  std::vector<MuonObj> & muons = muonColl->data();

  for (const auto & checkHlt : theNamesCheckHltMuMatchIdx) {
    int checkAlgoIndex = checkHlt.second; 
    if (checkAlgoIndex < 0) continue;
    bool isIsoAlgo = (theNamesAlgoHLT[checkAlgoIndex].find("Iso") != std::string::npos); 
    if( std::find(theFiredHLT.begin(), theFiredHLT.end(), checkAlgoIndex) == theFiredHLT.end() ) continue; 


    const std::vector<std::string> moduleLabels(theHltConfig.moduleLabels(checkAlgoIndex));
    unsigned int moduleFireIndex = theHltConfig.size(checkAlgoIndex)-2;
    // check if better way exists, this one is crazy....
    unsigned hltFilterIndex = triggerAOD->filterIndex( edm::InputTag ( moduleLabels[moduleFireIndex], "", "HLT") ); 
    if (hltFilterIndex >= triggerAOD->sizeFilters())  { std::cout <<" PROBLEM, wrong filter index, skip" << std::endl; continue;}

    trigger::Keys triggerKeys(triggerAOD->filterKeys(hltFilterIndex));

    unsigned nTriggers = triggerKeys.size();
//    std::cout <<" fired algo: " << theNamesAlgoHLT[checkAlgoIndex] <<", triggers: " << nTriggers<<std::endl;
    for (size_t iTrig = 0; iTrig < nTriggers; ++iTrig) {
      trigger::TriggerObject trigObject = triggerObjects[triggerKeys[iTrig]];
      for (auto & muon : muons) {
        double dR = reco::deltaR(muon, trigObject);
        if (dR < 0.1) { if (isIsoAlgo) muon.isMatchedIsoHlt = true; else  muon.isMatchedHlt = true; }
      }
    }
  } 

  return true;

}

bool MenuInspector::filter(edm::Event&ev, const edm::EventSetup&es)
{
/*
  theCounterIN++;
  if ( !filterL1(ev,es) ) return false;
  theCounterL1++;
  if ( !filterHLT(ev,es) ) return false;
  theCounterHLT++;
*/
  return true;
}



std::vector<unsigned int>  MenuInspector::runFiredAlgosL1(const edm::Event&ev, const edm::EventSetup&es) 
{
  std::vector<unsigned int> result;

  edm::Handle<GlobalAlgBlkBxCollection> ugt;
  ev.getByToken(theGlobalAlgToken, ugt);
  if (!ugt.isValid()) {
    std::cout << " PROBLEM, record uGT not OK " << std::endl;
    return result;
  }

  GlobalAlgBlk const * glbAlgBlk  = & ugt->at(0, 0);
  if (!glbAlgBlk) { std::cout << " PROBLEM, no glbAlgBlk, return " << std::endl; return result; }
//  if (glbAlgBlk) glbAlgBlk->print( std::cout);
  for (unsigned int idx = 0 ; idx < theNamesAlgoL1.size(); idx++) {
    bool isAccept = glbAlgBlk->getAlgoDecisionFinal(idx);
    if (isAccept) result.push_back(idx);
//    if (isAccept) std::cout <<" FIRED: "<< theNamesAlgoL1[idx] << std::endl;
  } 

  return result;
}

std::vector<unsigned int> MenuInspector::runFiredAlgosHLT(const edm::Event&ev, const edm::EventSetup&es) 
{
  std::vector<unsigned int> result;

  //
  // get trigger result
  //
  edm::Handle<edm::TriggerResults>   triggerResults;
  ev.getByLabel(edm::InputTag("TriggerResults","","HLT"), triggerResults);
  if (!triggerResults.isValid()) { 
    if (theWarnNoColl)std::cout << "firedAlgosHLT, PROBLEM, record not OK"<< std::endl; 
    return result; 
  }
  
  //
  // sanity check that names corresponding to trigger bits are as expected 
  //
  const edm::TriggerNames & triggerNames = ev.triggerNames(*triggerResults);
  assert(triggerResults->size()==theHltConfig.size());
  if (theTriggerParSetID != triggerNames.parameterSetID() ) {
    for (unsigned int triggerIndex =0; triggerIndex < theHltConfig.size()-1; ++triggerIndex) {   //skip "Final" decision indes
      std::string triggerName = theHltConfig.triggerName(triggerIndex);
      unsigned int triggerIndexTmp = theHltConfig.triggerIndex(triggerName);
      assert(triggerIndex==triggerIndexTmp);
      assert(triggerIndex==triggerNames.triggerIndex(triggerName));
    }
    theTriggerParSetID = triggerNames.parameterSetID();
  }

  //
  // fill result table
  //
  //unsigned int ntrig=0;
  for (unsigned int triggerIndex =0; triggerIndex < theHltConfig.size()-1; ++triggerIndex) {   //skip "Final" decision indes
    bool isAccept = triggerResults->accept(triggerIndex);
    if (isAccept) result.push_back(triggerIndex);
//    if (isAccept) std::cout <<  triggerIndex <<" ("<< theHltConfig.triggerName(triggerIndex)<<") "<< std::endl;;
    //    if (isAccept) ntrig++;
  }
  //std::cout <<"  --->  TOTAL NUMBER OF HLT TRIGGERS: " <<  ntrig << std::endl;
 
  return result;
}



/*

bool MenuInspector::filterL1(edm::Event&ev, const edm::EventSetup&es)
{
//  bool result = false;
  bool result = true;

  // reinitialiast Gt Menu,
  // not clear why it needs ev.
  int errorCode = 0;
  int  l1ConfCode = 0;
  static edm::RunNumber_t lastRun = 0; 
  theL1GtUtils.getL1GtRunCache(ev,es, false, true);

  if (ev.run() != lastRun) {
    lastRun = ev.run();
//    edm::InputTag tag("l1GtTriggerMenuLite");
//    theL1GtUtils.retrieveL1GtTriggerMenuLite(ev.getRun(), tag);
    bool status = theL1GtUtils.availableL1Configuration(errorCode,l1ConfCode);
    std::cout <<" GOT status: " <<status<<  " errorCode: " << errorCode<<" l1ConfCode: " << l1ConfCode<<std::endl;
    const L1GtTriggerMenuLite* menu = theL1GtUtils.ptrL1GtTriggerMenuLite(errorCode);
    if (errorCode) std::cout <<" ******Error CODE "<<errorCode<<std::endl;
    std::cout <<"NAME: " << menu->gtTriggerMenuName() << std::endl;
    //  for (L1GtTriggerMenuLite::CItL1Trig algo = menu->gtAlgorithmMap().begin(); algo!=menu->gtAlgorithmMap().end(); ++algo) 
    //      std::cout << "Id: " << algo->first << " Name: " << algo->second << std::endl;
    std::cout << *menu << std::endl;
  }


  //std::cout <<"Fired L1 Algorithms: " << std::endl;
  const L1GtTriggerMenuLite* menu = theL1GtUtils.ptrL1GtTriggerMenuLite(errorCode);
//  bool hasMuSeed = false;
  unsigned int ntrig = 0;
  for (unsigned int  i= 0; i<128; ++i) {
    const std::string * pname = menu->gtAlgorithmName(i,errorCode);
    if (errorCode) continue; // {std::cout<<"idx: "<<i<<" ERROR code(1) not 0, skip"<<std::endl; continue; }
    bool decision = theL1GtUtils.decisionAfterMask(ev, *pname, errorCode);
    if (errorCode) continue; //{std::cout<<"idx: "<<i<<" ERROR code(2) not 0, skip"<<std::endl; continue; }
    if (decision) {
     ntrig++;
     bool isMu = ( (*pname).find("Mu") != std::string::npos);
      std::cout<<"L1 idx: "<<i<<" "<<*pname; //<<" decision: "<<gtDecisionWord[i];
      if (!isMu) ;
 //     if (isMu) hasMuSeed = true;
      if (!isMu) result = true;
      //int triggerMask,prescaleFactor;
      //bool decisionBeforeMask, decisionAfterMask;
      //theL1GtUtils.l1Results(ev, *pname, decisionBeforeMask, decisionAfterMask, prescaleFactor, triggerMask);
      //theL1GtUtils.prescaleFactorSetIndex(ev, L1GtUtils::AlgorithmTrigger, errorCode);;
      std::cout <<std::endl;
    }
  }
  std::cout <<" total number of L1 triggers: "<<ntrig<<std::endl;
  //std::cout << "TRIGGER MENU NAME: "<<  l1GtUtils.l1TriggerMenu() << std::endl;
  //result = !hasMuSeed;
  //result=true;

  
  return result;
}
*/

/*
bool MenuInspector::filterHLT(edm::Event&ev, const edm::EventSetup&es)
{
  //bool result = false;
  bool result = true;

  // get event products
  edm::Handle<edm::TriggerResults>   triggerResultsHandle;
  ev.getByLabel(edm::InputTag("TriggerResults","","HLT"), triggerResultsHandle);
  if (!triggerResultsHandle.isValid()) {
    std::cout << "HLTEventAnalyzerAOD::analyze: Error in getting TriggerResults product from Event!" << std::endl;
    return 0;
  }
  assert(triggerResultsHandle->size()==theHltConfig.size());

//  bool hasMuSeed = false;
  std::cout <<"Number of HLT algorithms: "<<theHltConfig.size() << std::endl;
  unsigned int ntrig = 0;
  for (unsigned int triggerIndex =0; triggerIndex < theHltConfig.size()-1; ++triggerIndex) {   //skip "Final" decision indes
    std::string triggerName = theHltConfig.triggerName(triggerIndex);
    unsigned int triggerIndexTmp = theHltConfig.triggerIndex(triggerName);
    assert(triggerIndexTmp==triggerIndex);
    //AK assert(triggerIndex==ev.triggerNames(*triggerResultsHandle).triggerIndex(triggerName));
    bool isAccept = triggerResultsHandle->accept(triggerIndex);
//    if (true) {
    if (isAccept) {
      ntrig++;
      bool isMu = ( (triggerName.find("Mu") != std::string::npos) && (triggerName.find("Multi") == std::string::npos) );
//      std::cout <<triggerName;  if (!isMu) std::cout <<" <---- "; std::cout << std::endl;
//      if (isMu)  hasMuSeed = true;
      if (!isMu)  result = true;
       std::cout <<" HLT idx: "<<triggerIndex <<" "<<triggerName<< " Trigger path status:" 
              //<< " WasRun=" << triggerResultsHandle->wasrun(triggerIndex)
              << " Accept=" << triggerResultsHandle->accept(triggerIndex);
//              << " Error =" << triggerResultsHandle->error(triggerIndex)
//      if (isAccept) std::cout <<" <--";
      std::cout << std::endl;
    }
  }
  std::cout <<" total number of HLT triggers: "<<ntrig<<std::endl;
  return result;
}
*/

