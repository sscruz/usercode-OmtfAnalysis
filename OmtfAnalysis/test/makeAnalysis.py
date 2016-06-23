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
    skipRuns = cms.vuint32(275290),
    onlyRuns = cms.vuint32(),
  ),
  filterByAnaMuonDistribution = cms.bool(False),
  anaMuonDistribution = cms.PSet (
    requireAnyMuon = cms.bool(False),
    requireUnique  = cms.bool(True),
    requireOnlyOne = cms.bool(False),
    requireGlobal  = cms.bool(True),
    requireInner   = cms.bool(False),
    requireOuter   = cms.bool(False),
    ptMin  = cms.double(0.),
    etaMax = cms.double(2.1),
    minNumberOfMatchedStations = cms.uint32(0),
    minNumberRpcHits = cms.uint32(0),
    minNumberDtCscHits = cms.uint32(0),
  ),

  anaDataEmul =  cms.PSet(),
  anaEff =  cms.PSet(),     
)

process.p = cms.Path(process.omtfAnalysis)
