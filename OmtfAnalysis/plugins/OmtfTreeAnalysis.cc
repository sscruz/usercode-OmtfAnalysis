#include "OmtfTreeAnalysis.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TProfile.h"
#include "TH2F.h"
#include "TH1D.h"
#include "TGraphErrors.h"
#include "TF1.h"

#include "UserCode/OmtfDataFormats/interface/EventObj.h"
#include "UserCode/OmtfDataFormats/interface/TrackObj.h"
#include "UserCode/OmtfDataFormats/interface/MuonObj.h"
#include "UserCode/OmtfDataFormats/interface/MuonObjColl.h"
#include "UserCode/OmtfDataFormats/interface/L1Obj.h"
#include "UserCode/OmtfDataFormats/interface/L1ObjColl.h"
#include "UserCode/OmtfDataFormats/interface/TriggerMenuResultObj.h"

#include "UserCode/OmtfAnalysis/interface/AnaEvent.h"
#include "UserCode/OmtfAnalysis/interface/AnaMenu.h"
#include "UserCode/OmtfAnalysis/interface/AnaMuonDistribution.h"
#include "UserCode/OmtfAnalysis/interface/AnaEff.h"
#include "UserCode/OmtfAnalysis/interface/AnaDataEmul.h"
#include "UserCode/OmtfAnalysis/interface/AnaSecMuSelector.h"

OmtfTreeAnalysis::OmtfTreeAnalysis(const edm::ParameterSet & cfg)
  : theConfig(cfg),
    theAnaEvent(0), 
    theAnaMuonDistribution(0),
    theAnaMenu(0), 
    theAnaDataEmul(0), 
    theAnaEff(0),
    theAnaSecMu(0) 
{ 
  if (theConfig.exists("anaEvent")) theAnaEvent = new   AnaEvent(cfg.getParameter<edm::ParameterSet>("anaEvent") );
  if (theConfig.exists("anaMuonDistribution")) theAnaMuonDistribution = new AnaMuonDistribution( cfg.getParameter<edm::ParameterSet>("anaMuonDistribution"));
  if (theConfig.exists("anaMenu")) theAnaMenu = new AnaMenu(theConfig.getParameter<edm::ParameterSet>("anaMenu"));
  if (theConfig.exists("anaEff")) theAnaEff = new   AnaEff(cfg.getParameter<edm::ParameterSet>("anaEff") );
  if (theConfig.exists("anaDataEmul")) theAnaDataEmul = new AnaDataEmul(cfg.getParameter<edm::ParameterSet>("anaDataEmul"));
  if (theConfig.exists("anaSecMuSel")) theAnaSecMu = new AnaSecMuSelector(cfg.getParameter<edm::ParameterSet>("anaSecMuSel"));
}

void OmtfTreeAnalysis::beginJob()
{
  theHistos.SetOwner();

  if (theAnaEvent)            theAnaEvent->init(theHistos);
  if (theAnaMuonDistribution) theAnaMuonDistribution->init(theHistos);
  if (theAnaMenu)             theAnaMenu->init(theHistos);
  if (theAnaDataEmul)         theAnaDataEmul->init(theHistos);
  if (theAnaEff)              theAnaEff->init(theHistos);

}

void OmtfTreeAnalysis::beginRun(const edm::Run& ru, const edm::EventSetup& es)
{
  std::cout <<" OmtfTreeAnalysis::beginRun called" << std::endl;
}

void OmtfTreeAnalysis::analyze(const edm::Event&, const edm::EventSetup& es)
{
  //
  // prevent  multievent execution
  //
  static bool the_EMERGENCY_STOP = false;
  if (the_EMERGENCY_STOP) {
    std::cout <<"**  OmtfTreeAnalysis::analyze EMERGENCY_STOP fired, skip" << std::endl;
    return;
  }
  the_EMERGENCY_STOP = true;

  // 
  // define input chain
  //
  TChain chain("tOmtf");
  std::vector<std::string> treeFileNames = theConfig.getParameter<std::vector<std::string> >("treeFileNames");
  for (std::vector<std::string>::const_iterator it = treeFileNames.begin(); it != treeFileNames.end(); ++it)  chain.Add((*it).c_str() );


  //
  // prepare datastructures and branches
  //
  EventObj * event = 0;
  MuonObjColl * muonColl = 0;
  L1ObjColl* l1ObjColl = 0;
  TriggerMenuResultObj *bitsL1  = 0;
  TriggerMenuResultObj *bitsHLT = 0;
  

  chain.SetBranchAddress("event",&event);
//  chain.SetBranchAddress("muon",&muon);
  chain.SetBranchAddress("muonColl",&muonColl);
  chain.SetBranchAddress("l1ObjColl",&l1ObjColl);
  chain.SetBranchAddress("bitsL1",&bitsL1);
  chain.SetBranchAddress("bitsHLT",&bitsHLT);

  
  //
  // number of events
  //
  Int_t nentries= (Int_t) chain.GetEntries();
  std::cout <<" ENTRIES: " << nentries << std::endl;
 
  //
  // main loop
  //
  unsigned int lastRun = 0;
  for (int ev=0; ev<nentries; ev++) {


    chain.GetEntry(ev);
    if (theAnaMenu) theAnaMenu->updateMenu(bitsL1->names, bitsHLT->names);

    if ( (lastRun != (*event).run) || (ev%(std::max(nentries/10,1))==0)) { 
      lastRun = (*event).run; 
      std::cout <<"RUN:"    << std::setw(7) << (*event).run
                <<" event:" << std::setw(8) << ev
                <<" done:"  << std::setw(6)<< std::setiosflags(std::ios::fixed) << std::setprecision(2) << ev*100./nentries<<"%";
      std::cout<<std::endl; 
    }

    //
    // set reference muon
    // 
    MuonObj muon;
    if (theAnaSecMu) {
       muon = theAnaSecMu->run(event, *muonColl, l1ObjColl);
    } else { 
      std::vector<MuonObj> muons = *muonColl; 
      if (!muons.empty()) muon = muons[0];
    }

    //
    // Base distributions and Filters
    //
    // EVENT NUMBER, BX structure etc.
    if ( theAnaEvent && !theAnaEvent->filter(event) && theConfig.getParameter<bool>("filterByAnaEvent") ) continue;
    // ANALYSE AND FILTER KINEMCTICS
    if ( theAnaMuonDistribution && !theAnaMuonDistribution->filter(&muon) && theConfig.getParameter<bool>("filterByAnaMuonDistribution") ) continue;
    // ANALYSE AND FILTER TRIGGER MENU
    if ( theAnaMenu && !theAnaMenu->filter(event, &muon, bitsL1, bitsHLT) && theConfig.getParameter<bool>("filterByAnaMenu") ) continue;

/*
    if (!muon.isValid() ||  muon.pt() < 300 || muon.pt() > 400 ) continue;
    std::cout <<"---------------------------------------"<<std::endl;
    std::cout << *event << std::endl;
    if (muonColl) std::cout << *muonColl << std::endl;
    if (l1ObjColl)  std::cout << *l1ObjColl << std::endl; 
    if ( muon.isValid() ) std::cout <<" muon: " << muon << std::endl; 
    theAnaMenu->debug=true;
    theAnaEff->debug=true;
    theAnaMenu->filter(event, &muon, bitsL1, bitsHLT);
    theAnaMenu->debug=false;
    theAnaEff->debug=false;
*/


    //
    // Analyses
    //
    if (theAnaMuonDistribution) theAnaMuonDistribution->run(&muon);
    if (theAnaEff)      theAnaEff->run ( event, &muon, l1ObjColl); 
    if (theAnaDataEmul) theAnaDataEmul->run(event, l1ObjColl); 
     

  }
}

void OmtfTreeAnalysis::endJob()
{
  std::cout <<"ENDJOB, summaries:"<<std::endl;
  if (theAnaMenu) theAnaMenu->resume(theHistos);
  if (theAnaMenu) theAnaEvent->resume(theHistos);
  if (theAnaEff)  theAnaEff->resume(theHistos);
  if (theAnaDataEmul)  theAnaDataEmul->resume(theHistos);

  std::string histoFile = theConfig.getParameter<std::string>("histoFileName");
  TFile f(histoFile.c_str(),"RECREATE");
  theHistos.Write();
  f.Close();
  std::cout <<"END"<<std::endl;

  delete theAnaEvent;
  delete theAnaMuonDistribution;
  delete theAnaEff;
  delete theAnaMenu;

}
