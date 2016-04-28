import FWCore.ParameterSet.Config as cms
import os
import sys
import commands

process = cms.Process("Analysis")
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1))

process.analyze= cms.EDAnalyzer("OmtfLocalDaqAnalyzer", inFile = cms.string(sys.argv[2]))

process.p = cms.Path(process.analyze)

