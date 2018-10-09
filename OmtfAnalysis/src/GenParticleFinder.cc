#include "UserCode/OmtfAnalysis/interface/GenParticleFinder.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"


#include "UserCode/OmtfAnalysis/interface/Utilities.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TObjArray.h"
#include "TH1D.h"
#include "TH2D.h"


GenParticlefinder::GenParticlefinder(const edm::ParameterSet& cfg, edm::ConsumesCollector&& cColl)
  : lastEvent(0), lastRun(0), theConfig(cfg), 
    theAllParticles(0), theGenPart(0)
{ 

  edm::InputTag genCollTag =  theConfig.getParameter<edm::InputTag>("genColl");
  cColl.consumes<reco::GenParticleCollection>(genCollTag);

}

bool GenParticlefinder::run(const edm::Event &ev, const edm::EventSetup &es)
{

  if (lastEvent==ev.id().event() && lastRun==ev.run()) return false;
  lastEvent = ev.id().event() ;
  lastRun = ev.run();
  theGenPart = 0;
  theGenObjs.clear();

  //get Muon
  edm::Handle<reco::GenParticleCollection> genparticles;
  edm::InputTag genCollTag =  theConfig.getParameter<edm::InputTag>("genColl");
  ev.getByLabel( genCollTag, genparticles);
  if (!genparticles.isValid()) {
    return false;
  }
  theAllParticles = genparticles->size();
  
  for (reco::GenParticleCollection::const_iterator im = genparticles->begin(); im != genparticles->end(); ++im) {
    
    
    GenObj genObj(im->pt(),im->eta(),im->phi(),im->mass(),im->charge(),
		  im->pdgId(),im->status(),0);
    
    theGenObjs.push_back(genObj);
  }
  
  //
  // sort resulting container by pT, in descending order
  //
  std::sort(theGenObjs.begin(), theGenObjs.end(), []( const GenObj &m1, const GenObj &m2) { return m1.pt() > m2.pt(); } );
  
  return true;
}

