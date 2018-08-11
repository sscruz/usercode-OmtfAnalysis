#include "UserCode/OmtfAnalysis/interface/ClosestTrackFinder.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "UserCode/OmtfAnalysis/interface/Utilities.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TObjArray.h"
#include "TH1D.h"
#include "TH2D.h"

ClosestTrackFinder::ClosestTrackFinder(const edm::ParameterSet& cfg, edm::ConsumesCollector&& cColl)
  :  theConfig(cfg)
{

  edm::InputTag trackCollTag =  theConfig.getParameter<edm::InputTag>("trackColl");
  cColl.consumes<reco::TrackCollection>(trackCollTag);

}

reco::Track ClosestTrackFinder::result(const edm::Event &ev, const edm::EventSetup &es, double eta, double phi)
{
  reco::Track closest;
  //get Muon
  edm::Handle<reco::TrackCollection> tracks;
  edm::InputTag trackColl =  theConfig.getParameter<edm::InputTag>("trackColl");
  static bool warnNoColl = theConfig.getUntrackedParameter<bool>("warnNoColl", true);
  ev.getByLabel( trackColl, tracks);
  if (!tracks.isValid()) {
    if (warnNoColl) std::cout <<"** WARNING - no collection labeled: "<<trackColl <<std::endl;
    return closest;
  }
//  std::cout <<"SIZE of tracks: " <<  tracks->size() << std::endl;
  double dRmin = 99.;
  for (const auto & track : *tracks) {
    if (track.pt() < 2.5) continue;
    double dR =  reco::deltaR( track.eta(), track.phi(),  eta, phi);
    if (dR < dRmin) {
      dRmin = dR;
      closest = track;
    }
//    std::cout << track.pt()<<closest.recHitsSize()<<" "<<track.eta()<<" delta R:"<< dR << std::endl;
  }
//  if (closest.recHitsSize())std::cout <<"SEL: "<< closest.pt()<<" "<<closest.eta() << std::endl;
  return closest;
}
void ClosestTrackFinder::initHistos( TObjArray & histos)
{
}
