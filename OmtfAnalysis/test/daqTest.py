import FWCore.ParameterSet.Config as cms
import commands
import os

#from Configuration.StandardSequences.Eras import eras
#process = cms.Process('OmtfTree',eras.Run2_2016)

process = cms.Process('OmtfTree')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

#
# For processing single files insert lines with 'file:/PATH/FILE.root'
# alernatively one can use 'root://xrootd.unl.edu//store/express/Run2015A.....root'
# or                       '/store/express/Run2015A/ExpressPhysics/FEVT/...root'
# (there is 255 file limit though). Can be empty for crab.
#
process.source = cms.Source("PoolSource", 
fileNames = cms.untracked.vstring(
#  'file:/afs/cern.ch/work/k/konec/data/runs/run281214-Cosmics_E0D61D8B-E27F-E611-AF32-FA163E368DD1.root',
   'file:/afs/cern.ch/work/k/konec/data/runs/run283999-Cosmics_14208324-0A9B-E611-B786-02163E01437C.root',
                                  ),
skipEvents =  cms.untracked.uint32(45)
)

#
# import of standard configurations
#
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
#process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('EventFilter.L1TRawToDigi.bmtfDigis_cfi')
process.load('EventFilter.L1TRawToDigi.emtfStage2Digis_cfi')
process.load('EventFilter.L1TRawToDigi.gmtStage2Digis_cfi')
process.load('EventFilter.L1TXRawToDigi.twinMuxStage2Digis_cfi')

#
# set proper GlobalTag
#
#from Configuration.AlCa.GlobalTag import GlobalTag
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

#
# message logger
#
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.MessageLogger.cerr.threshold = cms.untracked.string('DEBUG')
process.MessageLogger.debugModules = cms.untracked.vstring()
process.MessageLogger.debugModules.append('muonRPCDigis')
process.MessageLogger.debugModules.append('omtfStage2Digis')
process.options = cms.untracked.PSet( wantSummary=cms.untracked.bool(False))

process.omtfStage2Digis = cms.EDProducer("OmtfUnpacker",
  InputLabel = cms.InputTag('rawDataCollector'),
)


#process.raw2digi_step = cms.Path(process.muonRPCDigis+process.csctfDigis+process.bmtfDigis+process.emtfStage2Digis+process.twinMuxStage2Digis+process.gmtStage2Digis+process.omtfStage2Digis)
process.raw2digi_step = cms.Path(process.omtfStage2Digis)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.schedule = cms.Schedule(process.raw2digi_step, process.endjob_step)

#print process.dumpPython();
print process.schedule



