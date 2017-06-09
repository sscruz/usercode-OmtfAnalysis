#include "UserCode/OmtfAnalysis/interface/AnaMuonDistribution.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TProfile.h"
#include "TObjArray.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "UserCode/OmtfDataFormats/interface/MuonObj.h"
#include "UserCode/OmtfAnalysis/interface/Utilities.h"

#include <cmath>

namespace {
  TH1D *hMuonPt_DIS, *hMuonEta_DIS, *hMuonPhi_DIS;
  TH1D *hMuonPt_MEN, *hMuonEta_MEN, *hMuonPhi_MEN;
  TH2D *hMuonPtVsEta_Tk, *hMuonPtVsEta_Ma, *hMuonPtVsEta_Gl;
}

AnaMuonDistribution::AnaMuonDistribution(const edm::ParameterSet& cfg)
  : ptMin( cfg.getParameter<double>("ptMin") ),
    etaMax (cfg.getParameter<double>("etaMax") ),
    chi2Norm (cfg.getParameter<double>("chi2Norm") ),
    minNumberOfMatchedStations( cfg.getParameter<unsigned int>("minNumberOfMatchedStations") ),
    minNumberTkHits( cfg.getParameter<uint>("minNumberTkHits") ),
    minNumberRpcHits( cfg.getParameter<uint>("minNumberRpcHits") ),
    minNumberDtCscHits( cfg.getParameter<unsigned int>("minNumberDtCscHits") ), 
    minNumberRpcDtCscHits( cfg.getParameter<unsigned int>("minNumberRpcDtCscHits") ), 
    requireUnique(cfg.getParameter<bool>("requireUnique")),
    requireGlobal(cfg.getParameter<bool>("requireGlobal")),
    requireInner(cfg.getParameter<bool>("requireInner")),
    requireOuter(cfg.getParameter<bool>("requireOuter")),
    requireLoose(cfg.getParameter<bool>("requireLoose")),
    requireMedium(cfg.getParameter<bool>("requireMedium")),
    requireTight(cfg.getParameter<bool>("requireTight")), 
    requireTkIso(cfg.getParameter<bool>("requireTkIso")), 
    requirePFIso(cfg.getParameter<bool>("requirePFIso")) 
{ }

void AnaMuonDistribution::init(TObjArray& histos) 
{
  hMuonPt_DIS  = new TH1D("hMuonPt_DIS","All global muons Pt;Glb.muon p_{T} [GeV];Muons / bin",L1PtScale::nPtBins,L1PtScale::ptBins);  histos.Add(hMuonPt_DIS);
  hMuonEta_DIS = new TH1D("hMuonEta_DIS","All global muons Eta;Glb.muon #eta;Muons / bin",96, -2.4, 2.4);  histos.Add(hMuonEta_DIS);
  hMuonPhi_DIS = new TH1D("hMuonPhi_DIS","All global muons Phi;Glb.muon #phi [rad];Muons / bin",90,-M_PI,M_PI);  histos.Add(hMuonPhi_DIS);

  hMuonPt_MEN  = new TH1D("hMuonPt_MEN","All global muons Pt;Glb.muon p_{T} [GeV];Muons / bin",L1PtScale::nPtBins,L1PtScale::ptBins); histos.Add(hMuonPt_MEN);
  hMuonEta_MEN = new TH1D("hMuonEta_MEN","All global muons Eta;Glb.muon #eta;Muons / bin",96, -2.4, 2.4);  histos.Add(hMuonEta_MEN);
  hMuonPhi_MEN = new TH1D("hMuonPhi_MEN","All global muons Phi;Glb.muon #phi [rad];Muons / bin",90,-M_PI,M_PI);  histos.Add(hMuonPhi_MEN);

  hMuonPtVsEta_Tk = new TH2D("hMuonPtVsEta_Tk","hMuonPtVsEta_Tk", L1RpcEtaScale::nEtaBins, L1RpcEtaScale::etaBins, L1PtScale::nPtBins, L1PtScale::ptBins); histos.Add(hMuonPtVsEta_Tk);
  hMuonPtVsEta_Ma = new TH2D("hMuonPtVsEta_Ma","hMuonPtVsEta_Ma", L1RpcEtaScale::nEtaBins, L1RpcEtaScale::etaBins, L1PtScale::nPtBins, L1PtScale::ptBins); histos.Add(hMuonPtVsEta_Ma);
  hMuonPtVsEta_Gl = new TH2D("hMuonPtVsEta_Gl","hMuonPtVsEta_Gl", L1RpcEtaScale::nEtaBins, L1RpcEtaScale::etaBins, L1PtScale::nPtBins, L1PtScale::ptBins); histos.Add(hMuonPtVsEta_Gl);

}
bool AnaMuonDistribution::filter(const MuonObj *muon)
{
//  std::cout << *muon << std::endl;

  if (requireUnique  && !muon->isUnique) return false;
  if (requireGlobal  && !muon->isGlobal()) return false;
  if (requireInner   && !muon->isTracker()) return false;
  if (requireOuter   && !muon->isOuter()) return false;
  if (requireLoose   && !muon->isLoose) return false;
  if (requireMedium  && !muon->isMedium) return false;
  if (requireTight   && !muon->isTight) return false;
  if (requireTkIso   && !muon->isTkIsolated) return false;
  if (requirePFIso   && !muon->isPFIsolated) return false;
  if (muon->chi2Norm >  chi2Norm) return false;
  if (muon->pt() < ptMin) return false;
  if (fabs(muon->eta()) > etaMax) return false;
  if (muon->nMatchedStations < minNumberOfMatchedStations) return false;
  if (muon->nTrackerHits < minNumberTkHits) return false;
  if (muon->nRPCHits < minNumberRpcHits) return false;
  if (muon->nDTHits + muon->nCSCHits < minNumberDtCscHits) return false;
  if (muon->nRPCHits + muon->nDTHits + muon->nCSCHits < minNumberRpcDtCscHits) return false;

  hMuonPt_DIS->Fill(muon->pt());
  hMuonEta_DIS->Fill(muon->eta());
  hMuonPhi_DIS->Fill(muon->phi());

  hMuonPtVsEta_Tk->Fill(muon->eta(), muon->pt());
  if (muon->isMatched() ) hMuonPtVsEta_Ma->Fill(muon->eta(), muon->pt());
  if (muon->isGlobal()) hMuonPtVsEta_Gl->Fill(muon->eta(), muon->pt());
  return true;
}

void AnaMuonDistribution::run(const MuonObj *muon)
{
//  std::cout << *muon << std::endl;
  if (hMuonPt_MEN)  hMuonPt_MEN->Fill(muon->pt());
  if (hMuonEta_MEN) hMuonEta_MEN->Fill(muon->eta());
  if (hMuonPhi_MEN) hMuonPhi_MEN->Fill(muon->phi());
}

