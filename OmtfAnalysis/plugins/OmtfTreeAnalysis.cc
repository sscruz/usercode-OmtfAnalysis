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
#include "UserCode/OmtfDataFormats/interface/L1Obj.h"
#include "UserCode/OmtfDataFormats/interface/L1ObjColl.h"

#include "UserCode/OmtfAnalysis/interface/AnaEvent.h"
#include "UserCode/OmtfAnalysis/interface/AnaDataEmul.h"

OmtfTreeAnalysis::OmtfTreeAnalysis(const edm::ParameterSet & cfg)
  : theConfig(cfg),
    theAnaEvent(0), theAnaDataEmul(0)
{ 
  if (theConfig.exists("anaEvent")) theAnaEvent = new   AnaEvent(cfg.getParameter<edm::ParameterSet>("anaEvent") );
                                    theAnaDataEmul = new AnaDataEmul(edm::ParameterSet());
   
}

void OmtfTreeAnalysis::beginJob()
{
  theHistos.SetOwner();

  if (theAnaEvent)            theAnaEvent->init(theHistos);
  if (theAnaDataEmul)         theAnaDataEmul->init(theHistos);
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
  L1ObjColl* l1ObjColl = 0;
  

  chain.SetBranchAddress("event",&event);
  chain.SetBranchAddress("l1ObjColl",&l1ObjColl);
  
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

    if ( (lastRun != (*event).run) || (ev%(std::max(nentries/10,1))==0)) { 
      lastRun = (*event).run; 
      std::cout <<"RUN:"    << std::setw(7) << (*event).run
                <<" event:" << std::setw(8) << ev
                <<" done:"  << std::setw(6)<< std::setiosflags(std::ios::fixed) << std::setprecision(2) << ev*100./nentries<<"%";
      std::cout<<std::endl; 
    }

    // EVENT NUMBER, BX structure etc.
    if ( theAnaEvent && !theAnaEvent->filter(event) && theConfig.getParameter<bool>("filterByAnaEvent") ) continue;

    if (theAnaDataEmul) theAnaDataEmul->run(l1ObjColl); 

    if (*l1ObjColl) {
      std::cout << *event << std::endl;
      std::cout << *l1ObjColl << std::endl; 
    }
     
  }
}

void OmtfTreeAnalysis::endJob()
{
  std::cout <<"ENDJOB, summaries:"<<std::endl;

  std::string histoFile = theConfig.getParameter<std::string>("histoFileName");
  TFile f(histoFile.c_str(),"RECREATE");
  theHistos.Write();
  f.Close();
  std::cout <<"END"<<std::endl;

  delete theAnaEvent;
}
