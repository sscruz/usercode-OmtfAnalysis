import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1))

process.omtfAnalysis = cms.EDAnalyzer("OmtfTreeAnalysis",
  histoFileName = cms.string("omtfAnalysis.root"),
  treeFileNames = cms.vstring("omtfTree.root"),
#  treeFileNames = cms.vstring("../jobs/ZMuC/omtfTree.root","../jobs/ZMuD/omtfTree.root"),
#   treeFileNames = cms.vstring( "../jobs/ExD/omtfTree.root", "../jobs/ExE/omtfTree.root"),

  filterByAnaEvent = cms.bool(True),
  anaEvent = cms.PSet(
    skipRuns = cms.vuint32(),
#   onlyRuns = cms.vuint32(276870),
  ),

#  anaSecMuSel = cms.PSet(
#    triggMuon = cms.PSet (
#      requireCharge = cms.int32(-1),
#      requireTight = cms.bool(True),
#      requireUnique = cms.bool(False),
#      requireHLT    = cms.bool(False),
#      requireIsoForHLTIso = cms.bool(False),
#      minAcceptMuPtVal = cms.double(0.),
#      minMatchStations = cms.uint32(2),
#      maxMuEtaVal = cms.double(2.4),
#      minAcceptL1PtVal = cms.double(18.),
#      maxL1DeltaR  = cms.double(0.3),
#      minAccepL1Q  = cms.int32(12), 
#    ),
#    probeMuon = cms.PSet (
#      requireCharge = cms.int32(1),
#      requireUnique = cms.bool(False),
#      requireLoose  = cms.bool(True),
#      requireMedium = cms.bool(True),
#      requireTight  = cms.bool(False),
#      requireTkIso  = cms.bool(False),
#      requirePfIso  = cms.bool(False),
#      minTrgMuDeltaR = cms.double(1.0),
#      maxMuEtaVal = cms.double(2.4),
#    ),
#  ), 

  filterByAnaMuonDistribution = cms.bool(False),
  anaMuonDistribution = cms.PSet (
    requireUnique  = cms.bool(True),
    requireGlobal  = cms.bool(True),
    requireInner   = cms.bool(False),
    requireOuter   = cms.bool(False),
    requireLoose   = cms.bool(True),
    requireMedium  = cms.bool(True),
    requireTight   = cms.bool(True),
    requireTkIso   = cms.bool(False),
    requirePFIso   = cms.bool(False),
    chi2Norm  = cms.double(2.),
    ptMin     = cms.double(3.0),
    etaMax    = cms.double(2.4),
    minNumberOfMatchedStations = cms.uint32(2),
    minNumberTkHits = cms.uint32(6),
    minNumberRpcHits = cms.uint32(0),
    minNumberDtCscHits = cms.uint32(0),
    minNumberRpcDtCscHits = cms.uint32(2),
  ),

  filterByAnaMenu = cms.bool(False),
  anaMenu = cms.PSet( #OR of conditions for L1 and separately for HLT
    acceptL1_OtherThanMu             = cms.bool(False),
    acceptL1_Mu                      = cms.bool(False),
    acceptL1_Names                   = cms.vstring("L1_ZeroBias","L1_ZeroBias_copy",
                                                   "L1_IsolatedBunch","L1_FirstCollisionInOrbit"),
    acceptHLT_OtherThanMuPhysics     = cms.bool(False),
    acceptHLT_L1                     = cms.bool(False),
    acceptHLT_Physics                = cms.bool(False),
    acceptHLT_Mu                     = cms.bool(False),
    acceptHLT_ZeroBias               = cms.bool(True),
    acceptHLT_Names                  = cms.vstring(
        "Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", "Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ",
        "Photon500", "Photon600",
        "MET600", "MET700",
        "PFMET500", "PFMET600",
        "Ele250_CaloIdVT_GsfTrkIdT", "Ele300_CaloIdVT_GsfTrkIdT",
        "HT2000", "HT2500"),
  ),

  anaDataEmul =  cms.PSet(),
  anaEff =  cms.PSet(),     
  anaTime = cms.PSet(),
)

process.p = cms.Path(process.omtfAnalysis)
