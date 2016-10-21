#include "UserCode/OmtfAnalysis/interface/BestMuonFinder.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "UserCode/OmtfAnalysis/interface/Utilities.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TObjArray.h"
#include "TH1D.h"
#include "TH2D.h"

//TH2D* hMuHitsCSCvsEta;

namespace {
bool isMediumMuon(const reco::Muon & recoMu) {
      bool goodGlob = recoMu.isGlobalMuon() && 
                      recoMu.globalTrack()->normalizedChi2() < 3 && 
                      recoMu.combinedQuality().chi2LocalPosition < 12 && 
                      recoMu.combinedQuality().trkKink < 20; 
      bool isMedium = muon::isLooseMuon(recoMu) && 
                      recoMu.innerTrack()->validFraction() > 0.49 && 
                      muon::segmentCompatibility(recoMu) > (goodGlob ? 0.303 : 0.451); 
      return isMedium; 
}
bool isTightMuon(const reco::Muon& muon){
  if(!muon.isPFMuon() || !muon.isGlobalMuon() ) return false;
  bool muID = muon::isGoodMuon(muon,muon::GlobalMuonPromptTight) && (muon.numberOfMatchedStations() > 1);
  bool hits = muon.innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5 &&
    muon.innerTrack()->hitPattern().numberOfValidPixelHits() > 0;
  bool ip = true;
  return muID && hits && ip;
}
}



BestMuonFinder::BestMuonFinder(const edm::ParameterSet& cfg, edm::ConsumesCollector&& cColl)
  : lastEvent(0), lastRun(0), theConfig(cfg), 
    theUnique(true), theIsLoose(false), theIsMedium(false), theIsTight(false), 
    theAllMuons(0), theMuon(0),
    theTrackerHits(0), theRPCHits(0), theDTHits(0), theCSCHits(0),
    hMuChi2Tk(0), hMuChi2Gl(0), hMuNHitsTk(0), 
    hMuPtVsEta(0), hMuHitsRPCvsCSC(0), hMuHitsRPCvsDT(0),
    hMuonPt_BMF(0),hMuonEta_BMF (0),hMuonPhi_BMF(0)
{ 

  edm::InputTag beamSpotTag =  theConfig.getParameter<edm::InputTag>("beamSpot");
  cColl.consumes<reco::BeamSpot>(beamSpotTag);

  edm::InputTag muonCollTag =  theConfig.getParameter<edm::InputTag>("muonColl");
  cColl.consumes<reco::MuonCollection>(muonCollTag);

}

bool BestMuonFinder::run(const edm::Event &ev, const edm::EventSetup &es)
{

  if (lastEvent==ev.id().event() && lastRun==ev.run()) return false;
  lastEvent = ev.id().event() ;
  lastRun = ev.run();
  theMuon = 0;
  theUnique = true;
  theIsLoose = false;
  theIsMedium = false;
  theIsTight= false;
  theMuonObjs.clear();


  //getBeamSpot
  edm::InputTag beamSpot =  theConfig.getParameter<edm::InputTag>("beamSpot");
  edm::Handle<reco::BeamSpot> bsHandle;
  ev.getByLabel( beamSpot, bsHandle);
  if (!bsHandle.isValid()) return false;
  math::XYZPoint reference =  (bsHandle.isValid())  ?  math::XYZPoint(bsHandle->x0(), bsHandle->y0(), bsHandle->z0())
                                                    :  math::XYZPoint(0.,0.,0.);


  //get Muon
  edm::Handle<reco::MuonCollection> muons;
  edm::InputTag muonColl =  theConfig.getParameter<edm::InputTag>("muonColl");
  static bool warnNoColl = theConfig.getUntrackedParameter<bool>("warnNoColl", true);
  ev.getByLabel( muonColl, muons);
  if (!muons.isValid()) {
    if (warnNoColl) std::cout <<"** WARNING - no collection labeled: "<<muonColl <<std::endl; 
    return false;
  }
  theAllMuons = muons->size();
//  std::cout <<"SIZE of muons: " <<  muons->size() << std::endl;
  
  for (reco::MuonCollection::const_iterator im = muons->begin(); im != muons->end(); ++im) {

//    std::cout << "HERE Muon:" <<" Glb: "<< im->isGlobalMuon() <<" pt="<<im->bestTrack()->pt() 
//              <<" eta="<<im->bestTrack()->eta() <<" phi="<<im->bestTrack()->phi() <<" chi2: "<<im->bestTrack()->normalizedChi2()
//              <<" isLoose: " << muon::isLooseMuon(*im)<<" isMedium: " << isMediumMuon(*im) << " isTight: " << isTightMuon(*im) 
//              <<std::endl;

    //
    // set type of muon
    //
    bool isLoose =  muon::isLooseMuon(*im);
    bool isMedium = isMediumMuon(*im);
    bool isTight = isTightMuon(*im);

    if ( fabs(im->bestTrack()->eta()) >  theConfig.getParameter<double>("maxAbsEta")) continue;
    if (im->bestTrack()->pt() < theConfig.getParameter<double>("minPt")) continue;
    if (im->numberOfMatchedStations() <  theConfig.getParameter<int>("minNumberOfMatchedStations")) continue;
    if (theConfig.getParameter<bool>("requireLoose") && !muon::isLooseMuon(*im) ) continue;

    if (    theConfig.getParameter<bool>("requireInnerTrack")) {
      if (!im->isTrackerMuon() || !im->innerTrack().isNonnull()) continue;
      if (im->innerTrack()->dxy(reference) >  theConfig.getParameter<double>("maxTIP")) continue;
      if (hMuonPt_BMF)   hMuonPt_BMF->Fill( im->innerTrack()->pt());
      if (hMuonEta_BMF) hMuonEta_BMF->Fill( im->innerTrack()->eta());
      if (hMuonPhi_BMF) hMuonPhi_BMF->Fill( im->innerTrack()->phi());
      if (hMuChi2Tk) hMuChi2Tk->Fill(im->innerTrack()->normalizedChi2());
      if (hMuPtVsEta) hMuPtVsEta->Fill(im->innerTrack()->eta(), im->innerTrack()->pt());
    }
    if (    theConfig.getParameter<bool>("requireOuterTrack")){ 
      if (!im->isStandAloneMuon() || !im->outerTrack().isNonnull())continue;
    }
    if ( theConfig.getParameter<bool>("requireGlobalTrack")) { 
	if (!im->isGlobalMuon() ||  !im->globalTrack().isNonnull()) continue;  
    }

    if (hMuChi2Gl && im->isGlobalMuon()) hMuChi2Gl->Fill(im->combinedMuon()->normalizedChi2());

//
// TMP TIGHT SELECTION FOR IVAN
//  if (! im->isGlobalMuon()) continue;
//  if (! (im->globalTrack()->normalizedChi2() < 10)) continue;
//  if (! (im->globalTrack()->hitPattern().numberOfValidMuonHits() > 0)) continue;
//  if (! (im->numberOfMatchedStations() > 1)) continue;
//  if (! (fabs(im->innerTrack()->dxy(reference)) < 0.2)) continue; 
//  if (! (im->track()->hitPattern().numberOfValidPixelHits() > 0)) continue;
//  if (! (im->track()->hitPattern().numberOfValidTrackerHits() > 10)) continue;
// TMP END OF TIGHT MUON SELECTION FROM IVAN

    //remove muons without valid hits in tk and mu system
    int nTrackerHits =0;
    int nRPCHits =0;
    int nDTHits =0;
    int nCSCHits =0;

    if (im->isGlobalMuon()) {
      const reco::HitPattern& hp = (im->combinedMuon())->hitPattern();
      nTrackerHits = hp.numberOfValidTrackerHits();
      nRPCHits = hp.numberOfValidMuonRPCHits();
      nDTHits = hp.numberOfValidMuonDTHits();
      nCSCHits = hp.numberOfValidMuonCSCHits();
      if (hMuNHitsTk) hMuNHitsTk->Fill(fabs(nTrackerHits)+1.e-3);
      if (nDTHits==0 && hMuHitsRPCvsCSC) hMuHitsRPCvsCSC->Fill(nCSCHits,nRPCHits);
      if (nCSCHits==0 && hMuHitsRPCvsDT) hMuHitsRPCvsDT->Fill(nDTHits,nRPCHits);
    } else {
      if(im->isTrackerMuon()) {
        const reco::HitPattern& hp = (im->innerTrack())->hitPattern();
        nTrackerHits = hp.numberOfValidTrackerHits();
      }
      if (im->isStandAloneMuon()) {
        const reco::HitPattern& hp = (im->standAloneMuon())->hitPattern();
        nRPCHits = hp.numberOfValidMuonRPCHits();
        nDTHits = hp.numberOfValidMuonDTHits();
        nCSCHits = hp.numberOfValidMuonCSCHits();
      }
    }

    if (nTrackerHits< theConfig.getParameter<int>("minNumberTrackerHits")) continue;
    if ( nRPCHits < theConfig.getParameter<int>("minNumberRpcHits")) continue;
    if ( nDTHits + nCSCHits < theConfig.getParameter<int>("minNumberDtCscHits")  ) continue;

    //
    // check isolation
    //
    bool isTkIsolated = false;
    if (im->isIsolationValid() && im->isolationR03().sumPt/im->pt() < theConfig.getParameter<double>("cutTkIsoRel") ) isTkIsolated = true;
    bool isPFIsolated = false;
    if (im->isPFIsolationValid() && 
           (   im->pfIsolationR04().sumChargedHadronPt 
             + std::max(0., im->pfIsolationR04().sumNeutralHadronEt + im->pfIsolationR04().sumPhotonEt - 0.5*im->pfIsolationR04().sumPUPt)
            )/im->pt() < theConfig.getParameter<double>("cutPFIsoRel")
        ) isPFIsolated = true;
    

    //
    // check if muon is unigue
    //
    bool isUnique = true;
    double muonEta = im->bestTrack()->eta();
    double muonPhi = im->bestTrack()->phi();
    for (reco::MuonCollection::const_iterator imm = muons->begin(); imm != muons->end(); ++imm) {
      if (&(*imm) == &(*im)) continue;
      if (theConfig.getParameter<bool>("looseUnique") && ! muon::isLooseMuon(*imm)) continue;
      if ( imm->bestTrack()->pt() <   theConfig.getParameter<double>("minPtUnique") ) continue;
      if ( fabs(reco::deltaPhi(muonPhi, imm->bestTrack()->phi())) > theConfig.getParameter<double>("deltaPhiUnique")) continue; 
      if ( fabs(muonEta-imm->bestTrack()->eta()) > theConfig.getParameter<double>("deltaEtaUnique")) continue;
      isUnique = false;
    }
   
    MuonObj muonObj;
    muonObj.isUnique = isUnique;
    muonObj.isLoose  = isLoose;
    muonObj.isMedium = isMedium;
    muonObj.isTight  = isTight;
    muonObj.nAllMuons = theAllMuons;
    muonObj.nRPCHits = nRPCHits;
    muonObj.nDTHits  = nDTHits;
    muonObj.nCSCHits = nCSCHits;
    muonObj.nTrackerHits = nTrackerHits;
    muonObj.nMatchedStations = im->numberOfMatchedStations();
    muonObj.chi2Norm = im->bestTrack()->normalizedChi2();
    muonObj.isTkIsolated = isTkIsolated;
    muonObj.isPFIsolated = isPFIsolated;
    muonObj.setKine(im->bestTrack()->pt(), im->bestTrack()->eta(), im->bestTrack()->phi(), im->bestTrack()->charge());
    muonObj.setBits(im->isGlobalMuon(), im->isTrackerMuon(), im->isStandAloneMuon(), im->isCaloMuon(), im->isMatchesValid());
    theMuonObjs.push_back(muonObj);
 
    if (!theMuon || (im->bestTrack()->pt() > theMuon->bestTrack()->pt()) ) {
      theMuon = &(*im);
      theTrackerHits = nTrackerHits;
      theRPCHits = nRPCHits;
      theDTHits = nDTHits;
      theCSCHits = nCSCHits;
      theUnique = isUnique;
      theIsLoose = isLoose;
      theIsMedium = isMedium;
      theIsTight = isTight;
    }
  }
 
  //
  // sort resulting container by pT, in descending order
  //
  std::sort(theMuonObjs.begin(), theMuonObjs.end(), []( const MuonObj &m1, const MuonObj &m2) { return m1.pt() > m2.pt(); } );

  return true;
}

void BestMuonFinder::initHistos( TObjArray & histos)
{
  hMuChi2Tk = new TH1D("hMuChi2Tk","hMuChi2Tk",100.,0.,10.); histos.Add(hMuChi2Tk);
  hMuChi2Gl = new TH1D("hMuChi2Gl","hMuChi2Gl",100.,0.,10.); histos.Add(hMuChi2Gl);
  hMuNHitsTk = new TH1D("hMuNHitsTk","hMuNHitsTk",30,0.,30.); histos.Add(hMuNHitsTk);
  hMuPtVsEta = new TH2D("hMuPtVsEta","hMuPtVsEta", L1RpcEtaScale::nEtaBins, L1RpcEtaScale::etaBins, L1PtScale::nPtBins, L1PtScale::ptBins); histos.Add(hMuPtVsEta);
  hMuHitsRPCvsCSC = new TH2D("hMuHitsRPCvsCSC","hMuHitsRPCvsCSC", 40,0.,40., 6,0.,6.); histos.Add(hMuHitsRPCvsCSC);
  hMuHitsRPCvsDT  = new TH2D("hMuHitsRPCvsDT","hMuHitsRPCvsDT", 30,0.,60., 6,0.,6.); histos.Add(hMuHitsRPCvsDT);
//  hMuHitsCSCvsEta = new TH2D("hMuHitsCSCvsEta","hMuHitsCSCvsEta",17, 0.8,2.5, 40,0.,40); histos.Add(hMuHitsCSCvsEta);
  hMuonPt_BMF  = new TH1D("hMuonPt_BMF","All global muons Pt;Glb.muon p_{T} [GeV];Muons / bin",L1PtScale::nPtBins,L1PtScale::ptBins);  histos.Add(hMuonPt_BMF);
  hMuonEta_BMF = new TH1D("hMuonEta_BMF","All global muons Eta;Glb.muon #eta;Muons / bin",84, -2.1, 2.1);  histos.Add(hMuonEta_BMF);
  hMuonPhi_BMF = new TH1D("hMuonPhi_BMF","All global muons Phi;Glb.muon #phi [rad];Muons / bin",90,-M_PI,M_PI);  histos.Add(hMuonPhi_BMF);

}
