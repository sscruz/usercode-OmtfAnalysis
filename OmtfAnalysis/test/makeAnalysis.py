import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1))

process.omtfAnalysis = cms.EDAnalyzer("OmtfTreeAnalysis",
  treeFileNames = cms.vstring("omtfTree.root"),
  histoFileName = cms.string("omtfAnalysis.root"),
  anaEvent = cms.PSet(
    skipRuns = cms.vuint32(),
  ),
)

process.p = cms.Path(process.omtfAnalysis)
