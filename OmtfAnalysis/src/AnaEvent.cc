#include "UserCode/OmtfAnalysis/interface/AnaEvent.h"
#include "TObjArray.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraphErrors.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "UserCode/OmtfDataFormats/interface/EventObj.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>

namespace { 
 TH1D *hEvent_BX;
} 

AnaEvent::AnaEvent(const edm::ParameterSet& cfg) 
  : debug(false) 
{

  if (cfg.exists("onlyRuns")) {
    theSkipRuns = cfg.getParameter<std::vector<unsigned int> >("skipRuns");
    std::sort(theSkipRuns.begin(), theSkipRuns.end());
  }
  
  if (cfg.exists("onlyRuns")) {
    theOnlyRuns =  cfg.getParameter<std::vector<unsigned int> >("onlyRuns");
    std::sort(theOnlyRuns.begin(), theOnlyRuns.end());
  }

//  std::ifstream infile( cfg.getParameter<std::string>("validBXes") );
//  while (infile.good() && infile.peek() != EOF) {
//    char c = infile.peek();
//    if ( c=='#') infile.ignore(256,'\n');  //comment
//    else if ( c==',' || c==' ' || c=='\n') infile.ignore();
//    else if ( (c >= '0') && (c <= '9') ) {int bx; infile >>bx;  theValidBX.set(bx);}
//    else { std::cout <<"Unexpected input file formatting, line is>>>"<<c<<"<<<"<<std::endl;infile.ignore(); }
//  }
//  infile.close();
}

bool AnaEvent::filter(EventObj* ev)

{
  // skip if wrong run
  if ( binary_search(theSkipRuns.begin(), theSkipRuns.end(), ev->run ) ) return  false;

  // if onlyRuns given and run not in list skip it. 
  if (theOnlyRuns.size() && !binary_search(theOnlyRuns.begin(), theOnlyRuns.end(), ev->run ) ) return  false;

  // skip if event duplication
  if ( theRunEvent.find(std::make_pair(ev->run, ev->id) ) == theRunEvent.end()) theRunEvent[std::make_pair(ev->run, ev->id)] = ev->time;
  else return false; 

  //control histograms

  hEvent_BX->Fill(ev->bx);
  
  return true;
}

void AnaEvent::resume(TObjArray& histos)
{
}

void AnaEvent::init(TObjArray& histos)
{
  hEvent_BX = new TH1D("hEvent_BX","hEvent_BX",3564,0., 3564.);  histos.Add( hEvent_BX);
}
