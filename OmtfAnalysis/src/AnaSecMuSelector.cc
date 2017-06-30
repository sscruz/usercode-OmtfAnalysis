#include "UserCode/OmtfAnalysis/interface/AnaSecMuSelector.h"

#include "TObjArray.h"
#include "TH1D.h"
#include "TH2D.h"
#include "UserCode/OmtfDataFormats/interface/MuonObj.h"
#include "UserCode/OmtfDataFormats/interface/EventObj.h"
#include "UserCode/OmtfDataFormats/interface/L1ObjColl.h"
#include "UserCode/OmtfAnalysis/interface/Utilities.h"
#include "DataFormats/Math/interface/deltaR.h"


#include <ostream>
#include <iostream>
#include <cmath>

namespace { }

AnaSecMuSelector::AnaSecMuSelector(const edm::ParameterSet& cfg)
  : debug(false), 
    theCfgTrg (   cfg.getParameter<edm::ParameterSet>("triggMuon") ),
    theCfgProbe ( cfg.getParameter<edm::ParameterSet>("probeMuon") )
{}

MuonObj AnaSecMuSelector::run( 
    const EventObj* ev, const std::vector<MuonObj> & muons, const L1ObjColl * l1Objs) 
{

  //
  // check for triggering muon
  //
  MuonObj trg;
  unsigned int imuon =0;
  for (const auto & muon : muons) {
    imuon++;
    if (   theCfgTrg.exists("requireCharge")
        && theCfgTrg.getParameter<int>("requireCharge") != muon.charge() ) continue;
    if (   theCfgTrg.exists("requireEtaSign")
        && theCfgTrg.getParameter<int>("requireEtaSign") != ((muon.eta() > 0) - (muon.eta() < 0)) ) continue;
    if (   theCfgTrg.exists("requireOutsideOMTF")
        && theCfgTrg.getParameter<bool>("requireOutsideOMTF") && !(fabs(muon.eta()) < 0.6 || fabs(muon.eta())> 1.5) ) continue;

    if (theCfgTrg.getParameter<bool>("requireTight") && !muon.isTight) continue;
    if (theCfgTrg.getParameter<bool>("requireUnique") && !muon.isUnique) continue;
    if (    theCfgTrg.getParameter<bool>("requireHLT")  
         && !muon.isMatchedHlt && !muon.isMatchedIsoHlt ) continue;
    if(     theCfgTrg.getParameter<bool>("requireIsoForHLTIso") && !muon.isMatchedHlt) {
        if (!muon.isMatchedIsoHlt) continue;
        if (!muon.isTkIsolated && !muon.isPFIsolated ) continue;
    }
    if (muon.nMatchedStations < theCfgTrg.getParameter<uint>("minMatchStations") ) continue;
    if (muon.pt() < theCfgTrg.getParameter<double>("minAcceptMuPtVal") ) continue;
    if (fabs(muon.eta()) > theCfgTrg.getParameter<double>("maxMuEtaVal")) continue;
    std::vector<L1Obj> uGMTs = l1Objs->selectByType(L1Obj::uGMT);
    for (const auto & ugmt : uGMTs) {
      if (ugmt.q < theCfgTrg.getParameter<int>("minAccepL1Q")) continue;
      if (ugmt.ptValue() < theCfgTrg.getParameter<double>("minAcceptL1PtVal") ) continue;
      double deltaR = reco::deltaR( ugmt.etaValue(), ugmt.phiValue(), muon.eta(), muon.phi());
      if (deltaR > theCfgTrg.getParameter<double>("maxL1DeltaR") ) continue;
      trg = muon;
      break; 
    } 
    if (trg.isValid()) break;  
  }
  if (!trg.isValid()) return MuonObj();

  //
  // check for probe muon
  //
  for (const auto & probe : muons) {
    if (   theCfgProbe.exists("requireCharge")
        && theCfgProbe.getParameter<int>("requireCharge") != probe.charge() ) continue;
    if (   theCfgProbe.exists("requireEtaSign")
        && theCfgProbe.getParameter<int>("requireEtaSign") != ((probe.eta() > 0) - (probe.eta() < 0)) ) continue;
    if (   theCfgProbe.exists("requireInsideOMTF")
        && theCfgProbe.getParameter<bool>("requireInsideOMTF") && !( fabs(probe.eta()) > 0.6 && fabs(probe.eta())< 1.5) ) continue;
    if (theCfgProbe.getParameter<bool>("requireUnique") && !probe.isUnique) continue;
    if (theCfgProbe.getParameter<bool>("requireLoose") && !probe.isLoose) continue;
    if (theCfgProbe.getParameter<bool>("requireMedium") && !probe.isMedium) continue;
    if (theCfgProbe.getParameter<bool>("requireTight") && !probe.isTight) continue;
    if (theCfgProbe.getParameter<double>("maxMuEtaVal") < fabs(probe.eta())) continue;
    if (reco::deltaR(trg,probe) < theCfgProbe.getParameter<double>("minTrgMuDeltaR")) continue;
    return probe; 
  }

  return MuonObj();
}
