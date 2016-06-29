import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1))

process.omtfAnalysis = cms.EDAnalyzer("OmtfTreeAnalysis",
  treeFileNames = cms.vstring("omtfTree.root"),
  histoFileName = cms.string("omtfAnalysis.root"),

  filterByAnaEvent = cms.bool(True),
  anaEvent = cms.PSet(
#    skipRuns = cms.vuint32(275290),
    skipRuns = cms.vuint32(),
#   onlyRuns = cms.vuint32(275319),
  ),

  filterByAnaMuonDistribution = cms.bool(True),
  anaMuonDistribution = cms.PSet (
    requireAnyMuon = cms.bool(False),
    requireUnique  = cms.bool(True),
    requireOnlyOne = cms.bool(False),
    requireGlobal  = cms.bool(False),
    requireInner   = cms.bool(False),
    requireOuter   = cms.bool(False),
    ptMin  = cms.double(0.),
    etaMax = cms.double(2.1),
    minNumberOfMatchedStations = cms.uint32(0),
    minNumberRpcHits = cms.uint32(0),
    minNumberDtCscHits = cms.uint32(0),
  ),

  filterByAnaMenu = cms.bool(True),
  anaMenu = cms.PSet(  #OR of confiditions for L1 and HLT
#   acceptL1_OtherThanMu             = cms.bool(False),
#   acceptL1_Mu                      = cms.bool(True),
#   acceptL1_Names                   = cms.vstring("L1_ZeroBias"),
#   acceptHLT_OtherThanMuPhysicsAlCa = cms.bool(False),
#   acceptHLT_L1                     = cms.bool(False),
#   acceptHLT_ZeroBias               = cms.bool(True),
#   acceptHLT_Physics                = cms.bool(False),
#   acceptHLT_Mu                     = cms.bool(False),


#   acceptL1_OtherThanMu             = cms.bool(False),
#   acceptL1_Mu                      = cms.bool(True),
#   acceptL1_Names                   = cms.vstring("L1_ZeroBias"),
#   acceptHLT_OtherThanMuPhysicsAlCa = cms.bool(False),
#   acceptHLT_L1                     = cms.bool(True),
#   acceptHLT_ZeroBias               = cms.bool(False),
#   acceptHLT_Physics                = cms.bool(False),
#   acceptHLT_Mu                     = cms.bool(False),


#   acceptL1_OtherThanMu             = cms.bool(True),
#   acceptL1_Mu                      = cms.bool(True),
#   acceptL1_Names                   = cms.vstring("L1_ZeroBias","L1_SingleMu16"),
#   acceptHLT_OtherThanMuPhysicsAlCa = cms.bool(True),
#   acceptHLT_L1                     = cms.bool(True),
#   acceptHLT_ZeroBias               = cms.bool(True),
#   acceptHLT_Physics                = cms.bool(True),
#   acceptHLT_Mu                     = cms.bool(True),

    acceptL1_OtherThanMu             = cms.bool(True),
    acceptL1_Mu                      = cms.bool(False),
    acceptL1_Names                   = cms.vstring("L1_ZeroBias"),
    acceptHLT_OtherThanMuPhysicsAlCa = cms.bool(True),
    acceptHLT_L1                     = cms.bool(False),
    acceptHLT_ZeroBias               = cms.bool(True),
    acceptHLT_Physics                = cms.bool(False),
    acceptHLT_Mu                     = cms.bool(False),
  ),


  anaDataEmul =  cms.PSet(),
  anaEff =  cms.PSet(),     
)

process.p = cms.Path(process.omtfAnalysis)
